#pragma once
#include <core/RoPrerequisites.h>
#include <core/task/RoTaskHandler.h>

roFORWARD_DECL_PTR(RoAudioManager);
roFORWARD_DECL_PTR(RoNetworkManager);

class RokLegend : public RoTaskHandler<RokLegend>
{
public:
    RokLegend(RoAudioManagerPtr audioManager, RoNetworkManagerPtr networkManager);

    void run();
    void exit() const;

private: // static
    static const RoString EXIT_TASK;
    static const std::string ROOT_FOLDER_FILE;

private:
    void stopGame(const RoTaskArgs& args);

    RoAudioManagerPtr mAudioManager;
    RoNetworkManagerPtr mNetworkManager;
    RoAtomicBool mCanRunGame;
};

roDEFINE_PTR(RokLegend);
