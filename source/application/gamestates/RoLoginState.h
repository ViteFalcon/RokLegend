#pragma once
#include <core/RoPrerequisites.h>

#include <network/RoNetServerType.h>
#include <network/packets/RoLoginFailed.h>

#include "RoGameState.h"

roFORWARD_DECL_PTR(RoButtonSound);
roFORWARD_DECL_PTR(RoLoginServer);

struct RoPacketReceivedEvent;
struct RoServerConnectedEvent;
struct RoServerConnectRequestFailedEvent;
struct RoServerDisconnectedEvent;

enum class RoLoginStage
{
    NONE,
    LOGIN_PROMPT,
    LOGIN_SERVER_CONNECTING,
    LOGIN_SERVER_READY,
    LOGIN_REQUEST_SENT,
    LOGIN_SUCCEEDED,
    LOGIN_FAILED,
    LOGIN_CANCELLED
};

RoString to_string(RoLoginStage stage);
std::ostream& operator << (std::ostream& stream, const RoLoginStage& stage);

class RoLoginState : public RoGameStateT<RoLoginState>
{
public:
    RoLoginState(RokLegendPtr game, RoBackgroundScorePtr backgroundScore, RoButtonSoundPtr buttonSound, RoLoginServerPtr loginServer);

protected:
    virtual void addTaskHandlers() override;
    virtual bool updateState(float timeSinceLastFrameInSecs) override;

private:
    void loginPrompt(const RoTaskArgs& args);
    RoLoginStage getCurrentStage() const;
    bool changeStage(RoLoginStage& expectedState, const RoLoginStage newState);

private:
    void serverConnectResponse(RoNetServerType type, RoOptionalString result);
    void loginResponse(optional<RoLoginFailed> result);

private: // static
    static const RoString LOGIN_PROMPT_TASK;
    static const RoString LOGIN_SUCCESS_TASK;
    static const RoString LOGIN_FAILED_TASK;
    static const RoString LOGIN_SERVER_CONNECT_FAILED_TASK;
    static const RoString LOGIN_SERVER_CONNECTED_TASK;
    static const RoString LOGIN_SERVER_DISCONNECTED_TASK;

private:
    using RoAtomicLoginStage = RoAtomic < RoLoginStage > ;

    RoAtomicLoginStage mStage;
    RoButtonSoundPtr mButtonSound;
    RoLoginServerPtr mLoginServer;
    RoOptionalString mUsername;
    RoOptionalString mPassword;
};