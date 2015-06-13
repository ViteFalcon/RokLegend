#pragma once
#include <audio/RoAudioManager.h>
#include <core/RoConfig.h>

class RoDataStore;

class RoBackgroundScore
{
public:
    RoBackgroundScore(RoAudioManagerPtr audioManager, RoConfigPtr config);
    ~RoBackgroundScore() = default;

    RoString getFileName() const
    {
        return mFileName;
    }

    void setScore(const RoString& filename);
    void setIsPaused(bool isPaused);
    bool isPaused() const;

    float getVolume();
    void setVolume(float volume);

private:
    RoDataStore& getDataStore();

private:
    RoAudioManagerPtr mAudioManager;
    const RoString mBgmFolder;
    RoString mFileName;
    RoAudioPtr mCurrentScore;
};

roDEFINE_PTR(RoBackgroundScore);