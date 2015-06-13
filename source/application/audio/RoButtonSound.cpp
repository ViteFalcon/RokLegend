#include "RoButtonSound.h"
#include <audio/RoAudio.h>
#include <storage/RoDataInfo.h>

const RoString RoButtonSound::sConfigKey{ L"str:ButtonSound" };

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
    return audioManager->getSound2D(fileName, soundDataStream, false);
}

void RoButtonSound::play()
{
    mAudioManager->playSound2D(mFileName, false);
}
