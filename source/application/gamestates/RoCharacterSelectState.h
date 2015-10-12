#pragma once
#include "RoGameState.h"

struct RoCharacterSelectArgs;
struct RoCharacterCreateArgs;
struct RoCharacterDeleteArgs;

roFORWARD_DECL_PTR(RoButtonSound);
roFORWARD_DECL_PTR(RoCharacterInformation);
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

private: // task schedulers
    void showMainMenu();
    void showSelectedCharacterMenu(RoCharacterInformationPtr character);
    void showCreateCharacterMenu(size_t slot);
    void showDeleteCharacterPrompt(size_t slot);
    void scheduleStage(Stage stage, RoString taskName, const RoTaskArgs& args);

private: // tasks
    void mainMenuPrompt(const RoTaskArgs& args);
    void selectCharacterPrompt(const RoCharacterSelectArgs& args);
    void createCharacterPrompt(const RoCharacterCreateArgs& args);
    void deleteCharacterPrompt(const RoCharacterDeleteArgs& args);

private: // callbacks
    void onSuccessfulCharacterCreation(const RoCharacterInformationPtr character);
    void onFailedCharacterCreation(const RoString& reason);

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
