#pragma once
#include <core/RoPrerequisites.h>
#include <core/task/RoTaskHandler.h>

#include "gamestates/RoGameState.h"

roFORWARD_DECL_PTR(RoAudioManager);
roFORWARD_DECL_PTR(RoNetworkManager);

class RokLegend : public RoTaskHandler<RokLegend>
{
public:
    RokLegend(
        RoAudioManagerPtr audioManager,
        RoNetworkManagerPtr networkManager,
        RoGameStateFactoryPtr gameStateFactory);

    void run();
    void setGameState(RoGameStates gameState);
    void exit() const;

private: // static
    static const RoString EXIT_TASK;
    static const std::string ROOT_FOLDER_FILE;

private:
    void stopGame(const RoTaskArgs& args);

private:
    using RoAtomicGameStateType = RoAtomic < RoGameStates > ;

    RoAudioManagerPtr mAudioManager;
    RoNetworkManagerPtr mNetworkManager;
    RoGameStateFactoryPtr mGameStateFactory;
    RoAtomicBool mCanRunGame;
    RoGameStates mGameState;
    RoAtomicGameStateType mNewGameState;
};

roDEFINE_PTR(RokLegend);
