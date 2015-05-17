////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include <core/RoFileSystem.h>

#include "RoBackgroundMusic.h"
#include <Audio/RoAudio.h>
#include <Audio/RoIrrKlangAudioManager.h>
//------------------------------------------------------------------------------
#define roBGM_FOLDER L"Game.Bgm.Folder"
#define roBGM_VOLUME_KEY L"Bgm.Volume"
//------------------------------------------------------------------------------
RoBackgroundMusicPtr RoBackgroundMusic::NewBackgroundMusic(const RoString& filePath)
{
    RoPath bgmFilePath = filePath;
    if (!RoFileSystem::FileExists(bgmFilePath))
    {
        static const RoString bgmFolderName = RoGame::GetConfigValue<RoString>(roBGM_FOLDER);
        static RoPath bgmFolder = bgmFolderName.asWStr();
        bgmFilePath = bgmFolder / bgmFilePath.leaf();
    }
    return RoBackgroundMusicPtr(new RoBackgroundMusic(bgmFilePath.wstring()));
}
//------------------------------------------------------------------------------
RoBackgroundMusic::RoBackgroundMusic(const RoString& filePath)
    : mFile(filePath)
    , mAudio(RoInstanceOf<RoIrrKlangAudioManager>::Get().getSound2D(filePath))
{
    mAudio->setIsPaused(false);
    mAudio->setIsLooped(true);
    RoDataStore& fileStore = RoGame::getSingleton().getRepository().getFileStore();
    if (!fileStore.hasKey(roBGM_VOLUME_KEY))
    {
        fileStore.putFloat(roBGM_VOLUME_KEY, 0.10f);
    }
    mAudio->setVolume(fileStore.getFloat(roBGM_VOLUME_KEY));
}
//------------------------------------------------------------------------------
RoBackgroundMusic::~RoBackgroundMusic()
{
    roLOG_INFO << "Audio use count: " << mAudio.use_count();
    mAudio.reset();
}