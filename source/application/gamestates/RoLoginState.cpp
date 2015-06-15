#include "RoLoginState.h"
#include "../audio/RoButtonSound.h"
#include "../audio/RoBackgroundScore.h"
#include "../RoGameBindings.h"
#include "../RokLegend.h"

#include <core/RoHashSet.h>
#include <core/RoLog.h>
#include <core/task/RoTaskManager.h>

#include <network/RoNetworkManager.h>
#include <network/events/RoPacketReceivedEvent.h>
#include <network/events/RoSendPacketToServerEvent.h>
#include <network/events/RoServerConnectRequestEvent.h>
#include <network/events/RoServerConnectedEvent.h>
#include <network/events/RoServerConnectionFailedEvent.h>
#include <network/events/RoServerDisconnectedEvent.h>
#include <network/packets/RoLoginCredentials.h>
#include <network/packets/RoLoginFailed.h>
#include <network/packets/RoLoginSuccessful.h>

const RoString RoLoginState::LOGIN_PROMPT_TASK{ L"LoginPrompt" };
const RoString RoLoginState::LOGIN_SUCCESS_TASK{L"AccountServerInfo"};
const RoString RoLoginState::LOGIN_FAILED_TASK{ L"MasterLoginError" };
const RoString RoLoginState::LOGIN_SERVER_CONNECT_FAILED_TASK{ L"LoginServerConnectionFailed" };
const RoString RoLoginState::LOGIN_SERVER_CONNECTED_TASK{ L"LoginServerConnected" };
const RoString RoLoginState::LOGIN_SERVER_DISCONNECTED_TASK{ L"LoginServerDisconnected" };

std::string roGetLoginStageString(RoLoginStage stage)
{
    switch (stage)
    {
    case RoLoginStage::NONE:
        return roSTRINGIFY(NONE);
    case RoLoginStage::LOGIN_PROMPT:
        return roSTRINGIFY(LOGIN_PROMPT);
    case RoLoginStage::LOGIN_SERVER_CONNECTING:
        return roSTRINGIFY(LOGIN_SERVER_CONNECTING);
    case RoLoginStage::LOGIN_REQUEST_SENT:
        return roSTRINGIFY(LOGIN_REQUEST_SENT);
    case RoLoginStage::LOGIN_SERVER_READY:
        return roSTRINGIFY(LOGIN_SERVER_READY);
    case RoLoginStage::LOGIN_CANCELLED:
        return roSTRINGIFY(LOGIN_CANCELLED);
    case RoLoginStage::LOGIN_FAILED:
        return roSTRINGIFY(LOGIN_FAILED);
    case RoLoginStage::LOGIN_SUCCEEDED:
        return roSTRINGIFY(LOGIN_SUCCEEDED);
    }
    std::stringstream ss;
    ss << "UKNOWN (" << as_integer(stage) << ")";
    return ss.str();
}

RoLoginState::RoLoginState(
    RokLegendPtr game,
    RoBackgroundScorePtr backgroundScore,
    RoButtonSoundPtr buttonSound,
    RoLoginSuccessfulPtr accountInfo)
    : RoGameStateT{ game, backgroundScore }
    , mButtonSound{ buttonSound }
    , mAccountInfo{ accountInfo }
    , mStage{ RoLoginStage::NONE }
{
}

void RoLoginState::addTaskHandlers()
{
    addTaskHandler(LOGIN_PROMPT_TASK, &RoLoginState::loginPrompt);
    addTaskHandler<RoPacketReceivedEvent>(LOGIN_SUCCESS_TASK, &RoLoginState::loginSuccessful);
    addTaskHandler<RoPacketReceivedEvent>(LOGIN_FAILED_TASK, &RoLoginState::loginFailed);
    addTaskHandler<RoServerConnectRequestFailedEvent>(LOGIN_SERVER_CONNECT_FAILED_TASK, &RoLoginState::loginServerConnectFailed);
    addTaskHandler<RoServerConnectedEvent>(LOGIN_SERVER_CONNECTED_TASK, &RoLoginState::loginServerConnected);
    addTaskHandler<RoServerDisconnectedEvent>(LOGIN_SERVER_DISCONNECTED_TASK, &RoLoginState::loginServerDisconnected);
}

bool RoLoginState::updateState(float timeSinceLastFrameInSecs)
{
    auto stage = getCurrentStage();
    bool continueState = true;
    switch (stage)
    {
    case RoLoginStage::NONE:
        changeStage(stage, RoLoginStage::LOGIN_PROMPT);
        roSCHEDULE_TASK_NAMED(LOGIN_PROMPT_TASK, RoEmptyArgs::INSTANCE);
        break;
    case RoLoginStage::LOGIN_CANCELLED:
        exitGame();
        continueState = false;
        break;
    }
    return continueState;
}

void RoLoginState::loginPrompt(const RoTaskArgs& args)
{
    auto currentStage = getCurrentStage();
    if (RoLoginStage::LOGIN_PROMPT != currentStage)
    {
        std::cerr << "Invalid state for login prompt(" << roGetLoginStageString(currentStage) << "). Resetting!" << std::endl;
        mStage.store(RoLoginStage::NONE);
        return;
    }

    auto username = readInput("Enter Username: ", false);
    if (!username)
    {
        mStage.store(RoLoginStage::LOGIN_CANCELLED);
        return;
    }
    mButtonSound->play();
    auto password = readInput("Enter Password: ", true);
    if (!password)
    {
        mStage.store(RoLoginStage::LOGIN_CANCELLED);
        return;
    }
    mButtonSound->play();
    mUsername = username;
    mPassword = password;

    if (changeStage(currentStage, RoLoginStage::LOGIN_SERVER_CONNECTING))
    {
        std::cout << "Connecting to login server... ";
        // FIXME: This should come from clientinfo.xml
        RoNetworkManager::Connect(RoNetServerType::LOGIN, "127.0.0.1", "6900");
    }
}

void RoLoginState::loginServerConnectFailed(const RoServerConnectRequestFailedEvent& args)
{
    std::cerr << "[FAILED]" << std::endl;
    std::cerr << "\tReason: " << args.reason << std::endl;
    auto currentState = getCurrentStage();
    changeStage(currentState, RoLoginStage::NONE);
}

void RoLoginState::loginServerConnected(const RoServerConnectedEvent& args)
{
    std::cout << "[SUCCESS]" << std::endl;
    auto currentStage = RoLoginStage::LOGIN_SERVER_CONNECTING;
    if (getCurrentStage() != currentStage)
    {
        std::cerr << "Punting logging in due to unexpected stage" << std::endl;
        return;
    }

    if (!mUsername || !mPassword)
    {
        std::cerr << "Login credential is not initialized! Aborting login!" << std::endl;
        RoNetworkManager::Disconnect(RoNetServerType::LOGIN);
        return;
    }

    auto loginPacket = std::make_shared<RoLoginCredentials>();
    loginPacket->setUsername(mUsername.get());
    loginPacket->setPassword(mPassword.get());
    mUsername.reset();
    mPassword.reset();

    if (changeStage(currentStage, RoLoginStage::LOGIN_REQUEST_SENT))
    {
        std::cout << "Logging in using credentials provided... ";
        RoNetworkManager::SendToServer(RoNetServerType::LOGIN, loginPacket);
    }
}

void RoLoginState::loginServerDisconnected(const RoServerDisconnectedEvent& args)
{
    std::cerr << "Disconnected from login server!" << std::endl;
    auto currentStage = getCurrentStage();
    changeStage(currentStage, RoLoginStage::NONE);
}

void RoLoginState::loginSuccessful(const RoPacketReceivedEvent& args)
{
    auto currentStage = RoLoginStage::LOGIN_REQUEST_SENT;
    std::cout << "[SUCCESS]" << std::endl;
    const auto& loginSuccess = args.packet->castTo<RoLoginSuccessful>();
    std::cout << "Account ID: " << loginSuccess.getId() << std::endl;
    std::cout << "Gender: " << to_string(loginSuccess.getGender()) << std::endl;
    std::cout << "Login ID1: " << loginSuccess.getLoginId1() << std::endl;
    std::cout << "Login ID2: " << loginSuccess.getLoginId2() << std::endl;
    std::cout << "Last Login IP: " << loginSuccess.getLastLoginIp() << std::endl;
    std::cout << "Last Login Time: " << loginSuccess.getLastLoginTime() << std::endl;
    mAccountInfo->fromProperties(loginSuccess.getProperties());
    if (changeStage(currentStage, RoLoginStage::LOGIN_SUCCEEDED))
    {
        mGame->setGameState(RoGameStates::CHARACTER_SERVER_SELECT);
    }
}

void RoLoginState::loginFailed(const RoPacketReceivedEvent& args)
{
    auto currentStage = RoLoginStage::LOGIN_REQUEST_SENT;
    std::cout << "[FAILED]" << std::endl;
    const auto& loginFailed = args.packet->castTo<RoLoginFailed>();
    std::cout << "\tReason: " << loginFailed.getReason() << std::endl;
    std::cout << "\tDetail: " << loginFailed.getDetail() << std::endl;
    if (changeStage(currentStage, RoLoginStage::LOGIN_SUCCEEDED))
    {
        RoNetworkManager::Disconnect(RoNetServerType::LOGIN);
    }
}

inline RoLoginStage RoLoginState::getCurrentStage() const
{
    return mStage.load();
}

inline bool RoLoginState::changeStage(RoLoginStage& expectedState, const RoLoginStage newState)
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

