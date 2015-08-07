#include "RoCharacterServerSelectState.h"
#include "../audio/RoButtonSound.h"
#include "../audio/RoBackgroundScore.h"
#include "../RoGameBindings.h"
#include "../RokLegend.h"
#include "../services/RoCharacterServerInterface.h"

#include <core/RoHashSet.h>
#include <core/RoLog.h>
#include <core/RoStringUtil.h>
#include <core/task/RoTaskManager.h>

#include <network/RoNetworkManager.h>
#include <network/events/RoPacketReceivedEvent.h>
#include <network/events/RoSendPacketToServerEvent.h>
#include <network/events/RoServerConnectRequestEvent.h>
#include <network/events/RoServerConnectedEvent.h>
#include <network/events/RoServerConnectionFailedEvent.h>
#include <network/events/RoServerDisconnectedEvent.h>
#include <network/packets/RoCharacterListing.h>
#include <network/packets/RoGameLogin.h>
#include <network/packets/RoLoginFailed.h>
#include <network/packets/RoLoginSuccessful.h>

const RoString RoCharacterServerSelectState::SERVER_SELECT_PROMPT_TASK{ L"CharacterServerSelectPrompt" };
const RoString RoCharacterServerSelectState::CHARACTER_SELECT_NOTIFICATION{ L"CharacterSelectNotification" };
const RoString RoCharacterServerSelectState::CHARACTER_SELECT_PAGES{ L"CharacterSelectPage" };
const RoString RoCharacterServerSelectState::BLOCKED_CHARACTER_INFORMATION{ L"BlockedCharacters" };
const RoString RoCharacterServerSelectState::CHARACTER_LISTING_TASK{ L"CharactersReceived" };
const RoString RoCharacterServerSelectState::LOGIN_FAILED_TASK{ L"GameLoginError" };
const RoString RoCharacterServerSelectState::PINCODE_SYSTEM_DISABLED{ L"PinCodeSystemDisabled" };
const RoString RoCharacterServerSelectState::CHARACTER_SERVER_CONNECT_FAILED_TASK{ L"CharacterServerConnectionFailed" };
const RoString RoCharacterServerSelectState::CHARACTER_SERVER_CONNECTED_TASK{ L"CharacterServerConnected" };
const RoString RoCharacterServerSelectState::CHARACTER_SERVER_DISCONNECTED_TASK{ L"CharacterServerDisconnected" };

RoCharacterServerSelectState::RoCharacterServerSelectState(
    RokLegendPtr game,
    RoBackgroundScorePtr backgroundScore,
    RoButtonSoundPtr buttonSound,
    RoCharacterServerInterfacePtr characterServer,
    RoLoginSuccessfulPtr accountInfo,
    RoCharacterListingPtr characterListing)
    : RoGameStateT{ game, backgroundScore }
    , mStage{ Stage::NONE }
    , mButtonSound{ buttonSound }
    , mCharacterServer{ characterServer }
    , mAccountInfo{ accountInfo }
    , mCharacterListing{ characterListing }
{
}

void RoCharacterServerSelectState::addTaskHandlers()
{
    addTaskHandler(SERVER_SELECT_PROMPT_TASK, &RoCharacterServerSelectState::serverSelectPrompt);
}

bool RoCharacterServerSelectState::updateState(float timeSinceLastFrameInSecs)
{
    auto stage = getCurrentStage();
    bool continueState = true;
    switch (stage)
    {
    case Stage::NONE:
        if (changeStage(stage, Stage::SERVER_SELECT_PROMPT))
        {
            roSCHEDULE_TASK_NAMED(SERVER_SELECT_PROMPT_TASK, RoEmptyArgs::INSTANCE);
        }
        break;
    case Stage::LOGIN_FAILED:
    case Stage::LOGIN_CANCELLED:
        mCharacterServer->disconnect();
        mGame->setGameState(RoGameStates::LOGIN);
        continueState = false;
        break;
    }
    return continueState;
}

void RoCharacterServerSelectState::serverSelectPrompt(const RoTaskArgs& args)
{
    roLOG_INFO << "Selecting server prompt...";
    auto currentStage = Stage::SERVER_SELECT_PROMPT;
    if (getCurrentStage() != currentStage)
    {
        std::cerr << "Invalid state for server-select prompt. Punting!" << std::endl;
        return;
    }

    auto hasValidEntry = false;
    auto selectedServer = 0;
    auto characterServers = mCharacterServer->getServerNames();
    do 
    {
        int i = 0;
        std::cout << std::endl;
        std::cout << "Select from the servers below" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        for (auto characterServer : characterServers)
        {
            std::cout << "\t" << i << ". " << characterServer << std::endl;
        }
        RoOptionalUInt32 serverSelected = readInteger("> Select server: ", false);
        if (!serverSelected.is_initialized())
        {
            changeStage(currentStage, Stage::LOGIN_CANCELLED);
            return;
        }
        mButtonSound->play();
        selectedServer = serverSelected.get();
        hasValidEntry = selectedServer < characterServers.size();
        if (!hasValidEntry)
        {
            std::cout << "Invalid server selection. Please try again!" << std::endl;
        }
    } while (!hasValidEntry);

    if (changeStage(currentStage, Stage::CHARACTER_SERVER_CONNECTING))
    {
        auto callback = std::bind(&RoCharacterServerSelectState::loginResult, this, std::placeholders::_1);
        mCharacterServer->loginToServerAtIndex(selectedServer, callback);
    }
}

RoCharacterServerSelectState::Stage RoCharacterServerSelectState::getCurrentStage() const
{
    return mStage.load();
}

bool RoCharacterServerSelectState::changeStage(Stage& expectedState, const Stage newState)
{
    const auto expected = expectedState;
    if (!mStage.compare_exchange_strong(expectedState, newState))
    {
        std::cerr << "Unexpected game-state found. Current: "
            << as_integer(expectedState)
            << ", Transition: " << as_integer(expected) << " -> " << as_integer(newState)
            << std::endl;
        return false;
    }
    return true;
}

void RoCharacterServerSelectState::loginResult(RoCharacterServerLoginResultPtr result)
{
    switch (result->getType())
    {
    case RoCharacterServerLoginResult::Type::CONNECT_FAILED:
        onConnectionFailed(std::dynamic_pointer_cast<RoCharacterServerConnectFailed>(result));
        break;
    case RoCharacterServerLoginResult::Type::LOGIN_FAILED:
        onLoginFailed(std::dynamic_pointer_cast<RoCharacterServerLoginFailed>(result));
        break;
    case RoCharacterServerLoginResult::Type::PIN_CODE_SYSTEM:
        onPinCodeSystem(std::dynamic_pointer_cast<RoCharacterServerPinCodeSystem>(result));
        break;
    case RoCharacterServerLoginResult::Type::SUCCESS:
        onLoginSuceeded(std::dynamic_pointer_cast<RoCharacterServerLoginSucceeded>(result));
        break;
    default:
        break;
    }
}

void RoCharacterServerSelectState::onConnectionFailed(RoCharacterServerConnectFailedPtr result)
{
    std::cout << "Failed to connect to character server.\n\tReason: " << result->getReason() << std::endl;
    auto currentStage = getCurrentStage();
    changeStage(currentStage, Stage::NONE);
}

void RoCharacterServerSelectState::onLoginFailed(RoCharacterServerLoginFailedPtr result)
{
    std::cout << "Failed to login to character server.\n\tReason: " << result->getReason() << std::endl;
    auto currentStage = getCurrentStage();
    changeStage(currentStage, Stage::LOGIN_FAILED);
}

void RoCharacterServerSelectState::onPinCodeSystem(RoCharacterServerPinCodeSystemPtr result)
{
    switch (result->getPinCodeRequest())
    {
    case RoPinCodeSystemRequest::DISABLED:
        roLOG_INFO << to_string(RoPinCodeSystemRequest::DISABLED).asWStr();
        break;
    default:
        roLOG_ERR << "Cannot handle Pincode system request: " << result->getPinCodeRequest();
        break;
    }
}

void RoCharacterServerSelectState::onLoginSuceeded(RoCharacterServerLoginSucceededPtr result)
{
    auto characterListing = result->getCharacterListing();
    if (characterListing->getCharacters().empty())
    {
        std::cout << "--- No characters available for this account" << std::endl;
    }
    else
    {
        for (auto characterInfo : characterListing->getCharacters())
        {
            std::cout
                << "[" << characterInfo->getSlot() << "] "
                << characterInfo->getName() << " "
                << characterInfo->getBaseLevel() << "/" << characterInfo->getJobLevel() << std::endl;
        }
    }
    auto currentStage = Stage::CHARACTER_SERVER_CONNECTING;
    // FIXME: This should propagate to the next state
    changeStage(currentStage, Stage::LOGIN_CANCELLED);
}

