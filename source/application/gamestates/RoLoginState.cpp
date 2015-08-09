#include "RoLoginState.h"
#include "../audio/RoBackgroundScore.h"
#include "../RoGameBindings.h"
#include "../RokLegend.h"
#include "../services/RoLoginServerInterface.h"
#include "../utilities/RoCli.h"

#include <core/RoHashSet.h>
#include <core/RoLog.h>
#include <core/task/RoTaskManager.h>

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

RoString to_string(RoLoginStage stage)
{
    switch (stage)
    {
    case RoLoginStage::NONE:
        return "NONE";
    case RoLoginStage::LOGIN_PROMPT:
        return "LOGIN_PROMPT";
    case RoLoginStage::LOGIN_SERVER_CONNECTING:
        return "LOGIN_SERVER_CONNECTING";
    case RoLoginStage::LOGIN_SERVER_READY:
        return "LOGIN_SERVER_READY";
    case RoLoginStage::LOGIN_REQUEST_SENT:
        return "LOGIN_REQUEST_SENT";
    case RoLoginStage::LOGIN_SUCCEEDED:
        return "LOGIN_SUCCEEDED";
    case RoLoginStage::LOGIN_FAILED:
        return "LOGIN_FAILED";
    case RoLoginStage::LOGIN_CANCELLED:
        return "LOGIN_CANCELLED";
    default:
        break;
    }
    std::stringstream error;
    error << "UNKNOWN LOGIN STAGE (" << as_integer(stage) << ")";
    return error.str();
}

std::ostream& operator<<(std::ostream& stream, const RoLoginStage& stage)
{
    return stream << to_string(stage);
}

RoLoginState::RoLoginState(
    RokLegendPtr game,
    RoBackgroundScorePtr backgroundScore,
    RoButtonSoundPtr buttonSound,
    RoLoginServerInterfacePtr loginServer)
    : RoGameStateT{ game, backgroundScore }
    , mButtonSound{ buttonSound }
    , mLoginServer{ loginServer }
    , mStage{ RoLoginStage::NONE }
{
}

void RoLoginState::addTaskHandlers()
{
    addTaskHandler(LOGIN_PROMPT_TASK, &RoLoginState::loginPrompt);
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

    auto username = RoCli::ReadString("Enter Username: ", false, mButtonSound);
    if (!username)
    {
        mStage.store(RoLoginStage::LOGIN_CANCELLED);
        return;
    }
    auto password = RoCli::ReadString("Enter Password: ", true, mButtonSound);
    if (!password)
    {
        mStage.store(RoLoginStage::LOGIN_CANCELLED);
        return;
    }
    mUsername = username;
    mPassword = password;

    if (changeStage(currentStage, RoLoginStage::LOGIN_SERVER_CONNECTING))
    {
        std::cout << "Connecting to login server... ";
        // FIXME: This should come from clientinfo.xml
        RoLoginServerInterface::ConnectCallback callback = std::bind(
            &RoLoginState::serverConnectResponse,
            this,
            std::placeholders::_1,
            std::placeholders::_2);
        mLoginServer->connect(L"127.0.0.1", L"6900", callback);
    }
}

void RoLoginState::serverConnectResponse(RoNetServerType type, RoOptionalString error)
{
    if (error.is_initialized())
    {
        std::cerr << "[FAILED]" << std::endl;
        std::cerr << "\tReason: " << error.get() << std::endl;
        auto currentState = getCurrentStage();
        changeStage(currentState, RoLoginStage::NONE);
        return;
    }
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
        mLoginServer->disconnect();
        return;
    }

    if (changeStage(currentStage, RoLoginStage::LOGIN_REQUEST_SENT))
    {
        std::cout << "Logging in using credentials provided... ";
        RoLoginServerInterface::LoginCallback callback = std::bind(&RoLoginState::loginResponse, this, std::placeholders::_1);
        mLoginServer->login(mUsername.get(), mPassword.get(), callback);
        mUsername.reset();
        mPassword.reset();
    }
}

void RoLoginState::loginResponse(optional<RoLoginFailed> error)
{
    auto currentStage = RoLoginStage::LOGIN_REQUEST_SENT;
    if (error)
    {
        std::cout << "[FAILED]" << std::endl;
        const auto& loginFailed = error.get();
        std::cout << "\tReason: " << loginFailed.getReason() << std::endl;
        std::cout << "\tDetail: " << loginFailed.getDetail() << std::endl;
        if (changeStage(currentStage, RoLoginStage::LOGIN_SUCCEEDED))
        {
            mLoginServer->disconnect();
        }
    }
    else if (changeStage(currentStage, RoLoginStage::LOGIN_SUCCEEDED))
    {
        mGame->setGameState(RoGameStates::CHARACTER_SERVER_SELECT);
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
            << expectedState
            << ", Transition: " << expected << " -> " << newState
            << std::endl;
        roLOG_ERR << "Unexpected game-state found. Current: "
            << expectedState
            << ", Transition: " << expected << " -> " << newState
            << std::endl;
        return false;
    }
    return true;
}

