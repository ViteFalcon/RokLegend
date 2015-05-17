////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "irrklang/RoIrrKlangAudioManager.h"
#include "RoAudio.h"
#include <core/RoFileSystem.h>
//------------------------------------------------------------------------------
#define roBGM_FOLDER_KEY L"Game.Bgm.Folder"
#define roBGM_VOLUME_KEY L"Bgm.Volume"
//------------------------------------------------------------------------------
RoAudioManagerPtr RoAudioManager::Get(const RoConfig& config)
{
    return shared_ptr < RoAudioManager > {new RoIrrKlangAudioManager(config)};
}
//------------------------------------------------------------------------------
RoAudioManager::RoAudioManager(const RoConfig& config)
    : mConfigs(config)
{
}
//------------------------------------------------------------------------------
RoAudioPtr RoAudioManager::getBackgroundMusic(const RoString& filename)
{
    RoPath bgmFilePath = filename;
    if (!RoFileSystem::FileExists(bgmFilePath))
    {
        static const RoString bgmFolderName = mConfigs.get(roBGM_FOLDER_KEY).as<RoString>();
        static const RoPath bgmFolder = bgmFolderName.asWStr();
        bgmFilePath = bgmFolder / bgmFilePath.leaf();
    }
    RoString resolvedFilename{ bgmFilePath.wstring() };
    auto audio = getSound2D(resolvedFilename);
    audio->setIsPaused(false);
    audio->setIsLooped(true);
    return audio;
}
//------------------------------------------------------------------------------
