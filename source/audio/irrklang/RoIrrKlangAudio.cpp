#include "RoIrrKlangAudio.h"
#include "../RoSoundStopReason.h"
#include "RoNullIrrKlangSound.h"
#include <core/RoLog.h>

RoIrrKlangAudio::RoIrrKlangAudio(
    bool is3d,
    const RoString& name,
    irrklang::ISoundEngine& engine,
    irrklang::ISoundSource* source,
    const RoVector3f& pos)
    : mIs3d(is3d)
    , mSound(0)
    , mPosition(pos)
{
    if (source)
    {
        if (mIs3d)
        {
            irrklang::vec3df irr_pos(pos.x(), pos.y(), pos.z());
            mSound = engine.play3D(source, irr_pos, false, true, true);
        }
        else
        {
            mSound = engine.play2D(source, false, true, true);
        }
        if (!mSound)
        {
            mSound = &irrklang::RoNullIrrKlangSound::INSTANCE;
        }
    }
    else
    {
        roLOG_ERR << "The sound file '" << name << "' is missing!";
        mSound = &irrklang::RoNullIrrKlangSound::INSTANCE;
    }
}
//------------------------------------------------------------------------------
RoIrrKlangAudio::~RoIrrKlangAudio()
{
    if (mSound)
    {
        if (!mSound->getIsPaused())
        {
            mSound->stop();
        }
        //mSound->stop();
        mSound->drop();
    }
}
//------------------------------------------------------------------------------
bool RoIrrKlangAudio::is3d() const
{
    return mIs3d;
}
//------------------------------------------------------------------------------
bool RoIrrKlangAudio::isEmpty() const
{
    return mSound->getPlayLength() == 0;
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::setIsLooped(bool looped)
{
    mSound->setIsLooped(looped);
}
//------------------------------------------------------------------------------
bool RoIrrKlangAudio::isLooped() const
{
    return mSound->isLooped();
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::setIsPaused(bool paused)
{
    mSound->setIsPaused(paused);
}
//------------------------------------------------------------------------------
bool RoIrrKlangAudio::isPaused() const
{
    return mSound->getIsPaused();
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::setPosition(const RoVector3f& position)
{
    irrklang::vec3df pos(position.x(), position.y(), position.z());
    mPosition = position;
    mSound->setPosition(pos);
}
//------------------------------------------------------------------------------
RoVector3f RoIrrKlangAudio::getPosition() const
{
    return mPosition;
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::setMinDistance(float minDistance)
{
    mSound->setMinDistance(minDistance);
}
//------------------------------------------------------------------------------
float RoIrrKlangAudio::getMinDistance() const
{
    return mSound->getMinDistance();
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::setMaxDistance(float maxDistance)
{
    mSound->setMaxDistance(maxDistance);
}
//------------------------------------------------------------------------------
float RoIrrKlangAudio::getMaxDistance() const
{
    return mSound->getMaxDistance();
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::setVolume(float volume)
{
    mSound->setVolume(volume);
}
//------------------------------------------------------------------------------
float RoIrrKlangAudio::getVolume() const
{
    return mSound->getVolume();
}
//------------------------------------------------------------------------------
bool RoIrrKlangAudio::isFinished() const
{
    return mSound->isFinished();
}
//------------------------------------------------------------------------------
size_t RoIrrKlangAudio::getLengthInMilliseconds() const
{
    return mSound->getPlayLength();
}
//------------------------------------------------------------------------------
float RoIrrKlangAudio::getLengthInSeconds() const
{
    return float(mSound->getPlayLength())*0.001f;
}
//------------------------------------------------------------------------------
size_t RoIrrKlangAudio::getPlayPositionInMilliseconds() const
{
    return mSound->getPlayPosition();
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::setPlayPositionInMilliseconds(uint32 position)
{
    mSound->setPlayPosition(position);
}
//------------------------------------------------------------------------------
float RoIrrKlangAudio::getPlayPositionInSeconds() const
{
    return float(mSound->getPlayPosition())*0.001f;
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::setPlayPositionInSeconds(float position)
{
    mSound->setPlayPosition(size_t(position*100.0f));
}
//------------------------------------------------------------------------------
float RoIrrKlangAudio::getPlayPosition() const
{
    return float(mSound->getPlayPosition()) / float(mSound->getPlayLength());
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::setPlayPosition(float position)
{
    if (0.0f > position)
    {
        position = 0.0f;
    }
    else if (1.0f < position) {
        position = 1.0f;
    }
    mSound->setPlayPosition(size_t(float(mSound->getPlayPosition())*position));
}
//------------------------------------------------------------------------------
void RoIrrKlangAudio::OnSoundStopped(irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE irrReason, void* userData)
{
    RoSoundStopReason reason = RoSoundStopReason::UNKNOWN;
    switch (irrReason)
    {
    case irrklang::ESEC_SOUND_FINISHED_PLAYING:
        reason = RoSoundStopReason::FINISHED_PLAYING;
        break;
    case irrklang::ESEC_SOUND_STOPPED_BY_USER:
        reason = RoSoundStopReason::STOPPED_BY_USER;
        break;
    case irrklang::ESEC_SOUND_STOPPED_BY_SOURCE_REMOVAL:
        reason = RoSoundStopReason::SOUND_SOURCE_DESTROYED;
        break;
    }
    //notifyStopped(reason);
}