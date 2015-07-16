#include "RoCharacterServerSelectState.h"
#include "../audio/RoButtonSound.h"
#include "../audio/RoBackgroundScore.h"
#include "../RoGameBindings.h"
#include "../RokLegend.h"

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
    RoLoginSuccessfulPtr accountInfo,
    RoCharacterListingPtr characterListing)
    : RoGameStateT{ game, backgroundScore }
    , mStage{ Stage::NONE }
    , mButtonSound{ buttonSound }
    , mAccountInfo{ accountInfo }
    , mCharacterListing{ characterListing }
{
}

void RoCharacterServerSelectState::addTaskHandlers()
{
    addTaskHandler(SERVER_SELECT_PROMPT_TASK, &RoCharacterServerSelectState::serverSelectPrompt);
    addTaskHandler<RoPacketReceivedEvent>(CHARACTER_LISTING_TASK, &RoCharacterServerSelectState::charactersReceived);
    addTaskHandler<RoPacketReceivedEvent>(CHARACTER_SELECT_NOTIFICATION, &RoCharacterServerSelectState::loginSuccessful);
    addTaskHandler<RoPacketReceivedEvent>(CHARACTER_SELECT_PAGES, &RoCharacterServerSelectState::characterSelectPages);
    addTaskHandler<RoPacketReceivedEvent>(BLOCKED_CHARACTER_INFORMATION, &RoCharacterServerSelectState::blockedCharacters);
    addTaskHandler<RoPacketReceivedEvent>(LOGIN_FAILED_TASK, &RoCharacterServerSelectState::loginFailed);
    addTaskHandler<RoPacketReceivedEvent>(PINCODE_SYSTEM_DISABLED, &RoCharacterServerSelectState::pincodeSystemDisabled);
    addTaskHandler<RoServerConnectRequestFailedEvent>(CHARACTER_SERVER_CONNECT_FAILED_TASK, &RoCharacterServerSelectState::characterServerConnectFailed);
    addTaskHandler<RoServerConnectedEvent>(CHARACTER_SERVER_CONNECTED_TASK, &RoCharacterServerSelectState::characterServerConnected);
    addTaskHandler<RoServerDisconnectedEvent>(CHARACTER_SERVER_DISCONNECTED_TASK, &RoCharacterServerSelectState::characterServerDisconnected);
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
    case Stage::LOGIN_CANCELLED:
        RoNetworkManager::Disconnect(RoNetServerType::CHARACTER);
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
    auto characterServers = mAccountInfo->getCharacterServers();
    do 
    {
        int i = 0;
        std::cout << std::endl;
        std::cout << "Select from the servers below" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        for (RoCharacterServerPtr characterServer : characterServers)
        {
            std::cout << "\t" << i << ". " << characterServer->getName() << " (" << characterServer->getNumberOfPlayers() << ")" << std::endl;
            std::cout << "\t\tRemote Address: " << characterServer->getIpAddress() << ":" << characterServer->getPortNumber() << std::endl;
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
        RoCharacterServerPtr characterServer = characterServers[selectedServer];
        RoString ipAddress = characterServer->getIpAddress();
        RoString portNumber = RoStringUtil::Format("%d", characterServer->getPortNumber());
        std::cout << "... connecting to server " << characterServer->getName() << "... ";
        RoNetworkManager::Connect(RoNetServerType::CHARACTER, ipAddress, portNumber);
    }
}

void RoCharacterServerSelectState::characterServerConnectFailed(const RoServerConnectRequestFailedEvent& args)
{
    std::cout << "[FAILED]" << std::endl << "\tReason: " << args.reason << std::endl;
    auto currentState = getCurrentStage();
    changeStage(currentState, Stage::LOGIN_CANCELLED);
}

void RoCharacterServerSelectState::characterServerConnected(const RoServerConnectedEvent& args)
{
    std::cout << "[SUCCESS]" << std::endl;
    auto currentStage = Stage::CHARACTER_SERVER_CONNECTING;
    if (getCurrentStage() != currentStage)
    {
        std::cerr << "Punting login due to unexpected stage" << std::endl;
        return;
    }

    auto loginPacket = std::make_shared<RoGameLogin>();
    loginPacket->setAccountId(mAccountInfo->getId());
    loginPacket->setLoginId(mAccountInfo->getLoginId1());
    loginPacket->setLoginId2(mAccountInfo->getLoginId2());
    loginPacket->setGender(mAccountInfo->getGender());

    if (changeStage(currentStage, Stage::LOGIN_REQUEST_SENT))
    {
        RoNetworkManager::SendToServer(RoNetServerType::CHARACTER, loginPacket);
    }
}

void RoCharacterServerSelectState::characterServerDisconnected(const RoServerDisconnectedEvent& args)
{
    std::cerr << "Disconnected from character server!" << std::endl;
    auto currentStage = getCurrentStage();
    changeStage(currentStage, Stage::LOGIN_CANCELLED);
}

void RoCharacterServerSelectState::charactersReceived(const RoPacketReceivedEvent& args)
{
    auto currentStage = Stage::LOGIN_REQUEST_SENT;
    const auto& characterListing = args.packet->castTo<RoCharacterListing>();
    std::cout << "Max Slots: " << characterListing.getMaxSlots().get() << std::endl;
    std::cout << "Available Slots: " << characterListing.getAvailableSlots().get() << std::endl;
    std::cout << "Premium Slots: " << characterListing.getPremiumSlots().get() << std::endl;
    if (characterListing.getCharacters().empty())
    {
        std::cout << "--- No characters available for this account" << std::endl;
    }
    else
    {
        for (auto characterInfo : characterListing.getCharacters())
        {
            std::cout
                << "[" << characterInfo->getSlot() << "] "
                << characterInfo->getName() << " "
                << characterInfo->getBaseLevel() << "/" << characterInfo->getJobLevel() << std::endl;
        }
    }
    mCharacterListing->fromProperties(characterListing.getProperties());
    if (changeStage(currentStage, Stage::LOGIN_SUCCEEDED))
    {
        // FIXME: This should propagate to the next state
        RoNetworkManager::Disconnect(RoNetServerType::CHARACTER);
    }
}

void RoCharacterServerSelectState::loginFailed(const RoPacketReceivedEvent& args)
{
    auto currentStage = Stage::LOGIN_REQUEST_SENT;
    std::cout << "Login FAILED!" << std::endl;
    std::cout << "\tReason: Rejected from server!" << std::endl;
    if (changeStage(currentStage, Stage::LOGIN_CANCELLED))
    {
        RoNetworkManager::Disconnect(RoNetServerType::CHARACTER);
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

void RoCharacterServerSelectState::pincodeSystemDisabled(const RoPacketReceivedEvent& args)
{
    auto pincodeSystemState = args.getProperties().get(_H("state")).as<uint16>();
    switch (pincodeSystemState)
    {
    case 0:
        std::cout << "Pincode System is disabled!" << std::endl;
        break;
    case 1:
        std::cout << "Ask for pin" << std::endl;
        break;
    case 2:
        std::cout << "Create new pin" << std::endl;
        break;
    case 3:
        std::cout << "Pin must be changed." << std::endl;
        break;
    case 4:
        std::cout << "Create new pin2" << std::endl;
        break;
    case 5:
        std::cout << "Client shows message string (1896)" << std::endl;
        break;
    case 6:
        std::cout << "Client shows a message (1897). Unable to use your KSSN number." << std::endl;
        break;
    case 7:
        std::cout << "Char select shows a button." << std::endl;
        break;
    case 8:
        std::cout << "Pincode was incorrect!" << std::endl;
        break;
    default:
        std::cout << "Unknown pincode system state: " << pincodeSystemState << std::endl;
        break;
    }
}

void RoCharacterServerSelectState::loginSuccessful(const RoPacketReceivedEvent& args)
{
    std::cout << "Login was SUCCESSFUL!" << std::endl;
}

void RoCharacterServerSelectState::characterSelectPages(const RoPacketReceivedEvent& args)
{
    auto pageCount = args.getProperties().get("page_count").as<uint32>();
    std::cout << "Received page count: " << pageCount << std::endl;
}

void RoCharacterServerSelectState::blockedCharacters(const RoPacketReceivedEvent& args)
{
    std::cout << "Received blocked characters list (ignoring for now)." << std::endl;
}

