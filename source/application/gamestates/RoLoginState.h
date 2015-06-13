#pragma once
#include <core/RoPrerequisites.h>

#include "RoGameState.h"

roFORWARD_DECL_PTR(RoButtonSound);

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

class RoLoginState : public RoGameStateT<RoLoginState>
{
public:
    RoLoginState(RokLegendPtr game, RoBackgroundScorePtr backgroundScore, RoButtonSoundPtr buttonSound);

protected:
    virtual void addTaskHandlers() override;
    virtual bool updateState(float timeSinceLastFrameInSecs) override;

private:
    void loginPrompt(const RoTaskArgs& args);
    RoLoginStage getCurrentStage() const;
    bool changeGameState(RoLoginStage& expectedState, const RoLoginStage newState);

private:
    void loginServerConnectFailed(const RoServerConnectRequestFailedEvent& args);
    void loginServerConnected(const RoServerConnectedEvent& args);
    void loginServerDisconnected(const RoServerDisconnectedEvent& args);
    void loginSuccessful(const RoTaskArgs& args);
    void loginFailed(const RoTaskArgs& args);

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
    RoOptionalString mUsername;
    RoOptionalString mPassword;
};