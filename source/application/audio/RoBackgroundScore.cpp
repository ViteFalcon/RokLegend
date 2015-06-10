#include "RoBackgroundScore.h"
#include <audio/RoAudio.h>
#include <core/RoFileSystem.h>
#include <storage/RoDataStore.h>

#define roBGM_FOLDER_KEY "Game.Bgm.Folder"
#define roBGM_LOGIN_SCORE_KEY "Game.Bgm.Login"
#define roBGM_VOLUME_KEY "Game.Bgm.Volume"

RoBackgroundScore::RoBackgroundScore(RoAudioManagerPtr audioManager, RoConfigPtr config)
    : mAudioManager(audioManager)
    , mBgmFolder(config->get(roBGM_FOLDER_KEY).as<RoString>())
{
    setScore(config->get(roBGM_LOGIN_SCORE_KEY).as<RoString>());
}

void RoBackgroundScore::setScore(const RoString& filename)
{
    if (mCurrentScore)
    {
        mCurrentScore->setIsPaused(true);
    }
    RoPath filePath = RoPath{ mBgmFolder } / RoPath{ filename };
    mCurrentScore = mAudioManager->getBackgroundMusic(filePath.native());
    mCurrentScore->setPlayPosition(0.0f);
    mCurrentScore->setIsPaused(false);
}

void RoBackgroundScore::setIsPaused(bool isPaused)
{
    if (mCurrentScore)
    {
        mCurrentScore->setIsPaused(isPaused);
    }
}

bool RoBackgroundScore::isPaused() const
{
    if (mCurrentScore)
    {
        return mCurrentScore->isPaused();
    }
    return false;
}

float RoBackgroundScore::getVolume()
{
    return getDataStore().getFloat(roBGM_VOLUME_KEY);
}

void RoBackgroundScore::setVolume(float volume)
{
    getDataStore().putFloat(roBGM_VOLUME_KEY, volume);
}

inline RoDataStore& RoBackgroundScore::getDataStore()
{
    auto& dataStore = RoDataStore::GetPersistentStore();
    static bool shouldStoreBeInitialized = true;
    if (shouldStoreBeInitialized)
    {
        dataStore.putFloat(roBGM_VOLUME_KEY, 1.0f);
        shouldStoreBeInitialized = false;
    }
    return dataStore;
}
