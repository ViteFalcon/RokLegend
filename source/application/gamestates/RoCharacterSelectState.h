#pragma once
#include "RoGameState.h"

roFORWARD_DECL_PTR(RoButtonSound);
roFORWARD_DECL_PTR(RoCharacterServerInterface);

class RoCharacterSelectState : public RoGameStateT < RoCharacterSelectState >
{
public:
    RoCharacterSelectState(
        RokLegendPtr game,
        RoBackgroundScorePtr backgroundScore,
        RoButtonSoundPtr buttonSound,
        RoCharacterServerInterfacePtr characterServer);

protected:
    virtual void addTaskHandlers() override;
    virtual bool updateState(float timeSinceLastFrameInSecs) override;

private:
    enum class Stage
    {
        NONE,
        MAIN_MENU,
        MAIN_MENU_CANCELLED,
        CHARACTER_LISTING,
        CHARACTER_LISTING_CANCELLED,
        CREATE_CHARACTER,
        CREATE_CHARACTER_CANCELLED,
        DELETE_CHARACTER,
        DELETE_CHARACTER_CANCELLED,
        CHARACTER_SELECTED,
        MAP_SERVER_CONNECTING,
        MAP_SERVER_CONNECT_FAILED,
        MAP_SERVER_CONNECT_SUCCEEDED,
        MAP_SERVER_DISCONNECTED
    };
    using RoAtomicStage = RoAtomic < Stage >;

private:
    Stage getCurrentStage() const;
    bool changeStage(Stage& expectedState, const Stage newState);
    std::string as_string(const Stage& stage);

private: // tasks
    void mainMenuPrompt(const RoTaskArgs& args);
    void selectCharacterPrompt(const RoTaskArgs& args);
    void createCharacterPrompt(const RoTaskArgs& args);
    void deleteCharacterPrompt(const RoTaskArgs& args);

private: // static
    static const RoString MAIN_MENU_PROMPT_TASK;
    static const RoString SELECT_CHARACTER_PROMPT_TASK;
    static const RoString CREATE_CHARACTER_PROMPT_TASK;
    static const RoString DELETE_CHARACTER_PROMPT_TASK;

private:
    RoAtomicStage mStage;
    RoButtonSoundPtr mButtonSound;
    RoCharacterServerInterfacePtr mCharacterServer;
};
