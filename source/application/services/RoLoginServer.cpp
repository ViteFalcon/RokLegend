#include "RoLoginServer.h"

#include <core/RoLog.h>

#include <network/events/RoPacketReceivedEvent.h>

#include <network/packets/RoLoginSuccessful.h>
#include <network/packets/RoLoginCredentials.h>

const RoString RoLoginServer::LOGIN_SUCCESS_TASK{ L"AccountServerInfo" };
const RoString RoLoginServer::LOGIN_FAILED_TASK{ L"MasterLoginError" };

RoLoginServer::RoLoginServer(RoLoginSuccessfulPtr accountInfo)
    : RoServerInterfaceT(RoNetServerType::LOGIN)
    , mAccountInfo(accountInfo)
{
}

void RoLoginServer::login(const RoString& username, const RoString& password, LoginCallback callback)
{
    mLoginCallback = callback;
    auto loginPacket = std::make_shared<RoLoginCredentials>();
    loginPacket->setUsername(username);
    loginPacket->setPassword(password);
    asyncSendPacket(loginPacket);
}

void RoLoginServer::addTaskHandlers()
{
    addTaskHandler<RoPacketReceivedEvent>(LOGIN_SUCCESS_TASK, &RoLoginServer::loginSuccessful);
    addTaskHandler<RoPacketReceivedEvent>(LOGIN_FAILED_TASK, &RoLoginServer::loginFailed);
}

void RoLoginServer::loginSuccessful(const RoPacketReceivedEvent& args)
{
    // TODO: Record account info
    const auto& loginSuccess = args.packet->castTo<RoLoginSuccessful>();
    mAccountInfo->fromProperties(loginSuccess.getProperties());
    invokeLoginCallback(optional < RoLoginFailed > {});
}

void RoLoginServer::loginFailed(const RoPacketReceivedEvent& args)
{
    optional<RoLoginFailed> loginFailed{ args.packet->castTo<RoLoginFailed>() };
    invokeLoginCallback(loginFailed);
}

RoString getLoginMessage(optional<RoLoginFailed> error)
{
    return error.is_initialized() ? error.get().getDetail() : "Login was successful!";
}

void RoLoginServer::invokeLoginCallback(optional<RoLoginFailed> error)
{
    if (mLoginCallback.is_initialized())
    {
        const auto& callback = mLoginCallback.get();
        roLOG_INFO << "Calling login-callback method with message: '" << getLoginMessage(error) << "'";
        callback(error);
        mLoginCallback.reset();
    }
    else
    {
        roLOG_ERR << "A login callback was not set to be invoked in RoLoginServer. Received message: '" << getLoginMessage(error) << "'";
    }
}
