#pragma once
#include <audio/RoAudioManager.h>
#include <core/RoConfig.h>

class RoDataStore;

class RoBackgroundScore
{
public:
    RoBackgroundScore(RoAudioManagerPtr audioManager, RoConfigPtr config);
    ~RoBackgroundScore() = default;

    void setScore(const RoString& filename);
    void setIsPaused(bool isPaused);
    bool isPaused() const;

    float getVolume();
    void setVolume(float volume);

private:
    RoDataStore& getDataStore();

private:
    RoAudioManagerPtr mAudioManager;
    RoString mBgmFolder;
    RoAudioPtr mCurrentScore;
};

roDEFINE_PTR(RoBackgroundScore);