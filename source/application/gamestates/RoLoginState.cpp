#include "RoLoginState.h"
#include "../audio/RoButtonSound.h"
#include "../audio/RoBackgroundScore.h"

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

RoLoginState::RoLoginState(RokLegendPtr game, RoBackgroundScorePtr backgroundScore, RoButtonSoundPtr buttonSound)
    : RoGameStateT{ game, backgroundScore }
    , mStage{ RoLoginStage::NONE }
    , mButtonSound{ buttonSound }
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
        changeGameState(stage, RoLoginStage::LOGIN_PROMPT);
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
    auto currentStage = RoLoginStage::LOGIN_PROMPT;
    if (getCurrentStage() != currentStage)
    {
        std::cerr << "Invalid state for login prompt. Punting!" << std::endl;
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

    if (changeGameState(currentStage, RoLoginStage::LOGIN_SERVER_CONNECTING))
    {
        // FIXME: This should come from clientinfo.xml
        RoNetworkManager::Connect(RoNetServerType::LOGIN, "127.0.0.1", "6900");
    }
}

void RoLoginState::loginServerConnectFailed(const RoServerConnectRequestFailedEvent& args)
{
    std::cerr << "Failed to connect to login server! Reason: " << args.reason << std::endl;
    auto currentState = getCurrentStage();
    changeGameState(currentState, RoLoginStage::NONE);
}

void RoLoginState::loginServerConnected(const RoServerConnectedEvent& args)
{
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

    if (changeGameState(currentStage, RoLoginStage::LOGIN_REQUEST_SENT))
    {
        RoNetworkManager::SendToServer(RoNetServerType::LOGIN, loginPacket);
    }
}

void RoLoginState::loginServerDisconnected(const RoServerDisconnectedEvent& args)
{
    std::cerr << "Disconnected from login server!" << std::endl;
    auto currentStage = getCurrentStage();
    changeGameState(currentStage, RoLoginStage::NONE);
}

void RoLoginState::loginSuccessful(const RoPacketReceivedEvent& args)
{
    auto currentStage = RoLoginStage::LOGIN_REQUEST_SENT;
    std::cout << "Login was SUCCESSFUL!" << std::endl;
    const auto& loginSuccess = args.packet->castTo<RoLoginSuccessful>();
    std::cout << "Account ID: " << loginSuccess.getId() << std::endl;
    std::cout << "Gender: " << to_string(loginSuccess.getGender()) << std::endl;
    std::cout << "Login ID1: " << loginSuccess.getLoginId1() << std::endl;
    std::cout << "Login ID2: " << loginSuccess.getLoginId2() << std::endl;
    std::cout << "Last Login IP: " << loginSuccess.getLastLoginIp() << std::endl;
    std::cout << "Last Login Time: " << loginSuccess.getLastLoginTime() << std::endl;
    std::cout << "Character Server Info" << std::endl;
    std::cout << "=====================" << std::endl;
    for (RoCharacterServerPtr server : loginSuccess.getCharacterServers())
    {
        std::cout << server->getName() << " (" << server->getNumberOfPlayers() << ")" << std::endl;
        std::cout << "\tServer Type: " << to_string(server->getServerType()) << std::endl;
        std::cout << "\tIP Address: " << server->getIpAddress() << std::endl;
        std::cout << "\tPort Number: " << server->getPortNumber() << std::endl;
    }
    if (changeGameState(currentStage, RoLoginStage::LOGIN_SUCCEEDED))
    {
        // FIXME: This should propagate to the next state
        RoNetworkManager::Disconnect(RoNetServerType::LOGIN);
    }
}

void RoLoginState::loginFailed(const RoPacketReceivedEvent& args)
{
    auto currentStage = RoLoginStage::LOGIN_REQUEST_SENT;
    std::cout << "Login FAILED!" << std::endl;
    const auto& loginFailed = args.packet->castTo<RoLoginFailed>();
    std::cout << "\tReason: " << loginFailed.getReason() << std::endl;
    std::cout << "\tDetail: " << loginFailed.getDetail() << std::endl;
    if (changeGameState(currentStage, RoLoginStage::LOGIN_SUCCEEDED))
    {
        RoNetworkManager::Disconnect(RoNetServerType::LOGIN);
    }
}

RoLoginStage RoLoginState::getCurrentStage() const
{
    return mStage.load();
}

bool RoLoginState::changeGameState(RoLoginStage& expectedState, const RoLoginStage newState)
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

