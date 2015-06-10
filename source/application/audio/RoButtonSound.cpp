#include "RoButtonSound.h"
#include <audio/RoAudio.h>
#include <storage/RoDataInfo.h>

const RoString RoButtonSound::sConfigKey{ L"str:ButtonSound" };
const RoString RoButtonSound::sFriendlyName{ L"buttonSound.wav" };

RoButtonSound::RoButtonSound(RoAudioManagerPtr audioManager, RoGrf2Ptr grf)
    : mAudioManager(audioManager)
    , mFileName(retrieveFileName())
    , mAudio(retrieveAudio(audioManager, grf, mFileName))
{
}

RoString RoButtonSound::retrieveFileName()
{
    return RoDataInfo::Get().getValue(sConfigKey).value();
}

RoAudioPtr RoButtonSound::retrieveAudio(RoAudioManagerPtr audioManager, RoGrf2Ptr grf, RoString fileName)
{
    auto soundDataStream = grf->getFileContentsOf(fileName);
    return audioManager->getSound2D(sFriendlyName, soundDataStream, false);
}

void RoButtonSound::play()
{
    mAudioManager->playSound2D(sFriendlyName, false);
}
