#pragma once
#include "RoServerInterface.h"

#include <network/packets/RoLoginFailed.h>

struct RoPacketReceivedEvent;
roFORWARD_DECL_PTR(RoLoginSuccessful);

class RoLoginServerInterface : public RoServerInterfaceT<RoLoginServerInterface>
{
public:
    RoLoginServerInterface(RoLoginSuccessfulPtr accountInfo);
    ~RoLoginServerInterface() = default;

    using LoginCallback = std::function < void(optional<RoLoginFailed>) > ;
    void login(const RoString& username, const RoString& password, LoginCallback callback);

private:
    virtual void addTaskHandlers() override;

    void loginSuccessful(const RoPacketReceivedEvent& args);
    void loginFailed(const RoPacketReceivedEvent& args);
    void invokeLoginCallback(optional<RoLoginFailed> error);
private: // static
    static const RoString LOGIN_SUCCESS_TASK;
    static const RoString LOGIN_FAILED_TASK;

private:
    optional<LoginCallback> mLoginCallback;
    RoLoginSuccessfulPtr mAccountInfo;
};