#pragma once
#include <audio/RoAudioManager.h>
#include <archive/RoGrf.h>

class RoButtonSound
{
public:
    RoButtonSound(RoAudioManagerPtr audioManager, RoGrf2Ptr grf);
    ~RoButtonSound() = default;

    void play();

private: // static
    static const RoString sConfigKey;
    static const RoString sFriendlyName;

    static RoString retrieveFileName();
    static RoAudioPtr retrieveAudio(RoAudioManagerPtr audioManager, RoGrf2Ptr grf, RoString fileName);

private:
    RoAudioManagerPtr mAudioManager;
    const RoString mFileName;
    RoAudioPtr mAudio;
};

roDEFINE_PTR(RoButtonSound);