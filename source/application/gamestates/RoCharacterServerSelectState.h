#pragma once
#include <core/RoPrerequisites.h>

#include "RoGameState.h"

roFORWARD_DECL_PTR(RoButtonSound);
roFORWARD_DECL_PTR(RoCharacterListing);
roFORWARD_DECL_PTR(RoCharacterServerConnectFailed);
roFORWARD_DECL_PTR(RoCharacterServerInterface);
roFORWARD_DECL_PTR(RoCharacterServerLoginFailed);
roFORWARD_DECL_PTR(RoCharacterServerLoginResult);
roFORWARD_DECL_PTR(RoCharacterServerLoginSucceeded);
roFORWARD_DECL_PTR(RoCharacterServerPinCodeSystem);
roFORWARD_DECL_PTR(RoLoginSuccessful);

struct RoPacketReceivedEvent;
struct RoServerConnectedEvent;
struct RoServerConnectRequestFailedEvent;
struct RoServerDisconnectedEvent;

class RoCharacterServerSelectState : public RoGameStateT<RoCharacterServerSelectState>
{
public:
    RoCharacterServerSelectState(
        RokLegendPtr game,
        RoBackgroundScorePtr backgroundScore,
        RoButtonSoundPtr buttonSound,
        RoCharacterServerInterfacePtr characterServer,
        RoLoginSuccessfulPtr accountInfo,
        RoCharacterListingPtr characterListing);

protected:
    virtual void addTaskHandlers() override;
    virtual bool updateState(float timeSinceLastFrameInSecs) override;

private:
    enum class Stage
    {
        NONE,
        SERVER_SELECT_PROMPT,
        CHARACTER_SERVER_CONNECTING,
        CHARACTER_SERVER_READY,
        LOGIN_REQUEST_SENT,
        LOGIN_SUCCEEDED,
        LOGIN_FAILED,
        LOGIN_CANCELLED
    };

    void serverSelectPrompt(const RoTaskArgs& args);
    Stage getCurrentStage() const;
    bool changeStage(Stage& expectedState, const Stage newState);

private:
    void loginResult(RoCharacterServerLoginResultPtr result);
    void onConnectionFailed(RoCharacterServerConnectFailedPtr result);
    void onLoginFailed(RoCharacterServerLoginFailedPtr result);
    void onPinCodeSystem(RoCharacterServerPinCodeSystemPtr result);
    void onLoginSuceeded(RoCharacterServerLoginSucceededPtr result);
private: // static
    static const RoString SERVER_SELECT_PROMPT_TASK;
    static const RoString CHARACTER_SELECT_NOTIFICATION;
    static const RoString CHARACTER_SELECT_PAGES;
    static const RoString BLOCKED_CHARACTER_INFORMATION;
    static const RoString CHARACTER_LISTING_TASK;
    static const RoString LOGIN_FAILED_TASK;
    static const RoString PINCODE_SYSTEM_DISABLED;
    static const RoString CHARACTER_SERVER_CONNECT_FAILED_TASK;
    static const RoString CHARACTER_SERVER_CONNECTED_TASK;
    static const RoString CHARACTER_SERVER_DISCONNECTED_TASK;

private:
    using RoAtomicStage = RoAtomic < Stage > ;

    RoAtomicStage mStage;
    RoButtonSoundPtr mButtonSound;
    RoCharacterServerInterfacePtr mCharacterServer;
    RoLoginSuccessfulPtr mAccountInfo;
    RoCharacterListingPtr mCharacterListing;
};