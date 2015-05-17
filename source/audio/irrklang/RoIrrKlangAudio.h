////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2015
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../RoAudio.h"
#include <irrKlang.h>

class RoIrrKlangAudio : public RoAudio, public irrklang::ISoundStopEventReceiver
{
public:
    RoIrrKlangAudio(
        bool is3d,
        const RoString& name,
        irrklang::ISoundEngine& engine,
        irrklang::ISoundSource* source,
        const RoVector3f& pos);
    ~RoIrrKlangAudio();

    virtual bool is3d() const override;

    virtual bool isEmpty() const override;

    virtual void setIsLooped(bool looped) override;
    virtual bool isLooped() const override;

    virtual void setIsPaused(bool paused) override;
    virtual bool isPaused() const override;

    virtual void setPosition(const RoVector3f& position) override;
    virtual RoVector3f getPosition() const override;

    virtual void setMinDistance(float minDistance) override;
    virtual float getMinDistance() const override;

    virtual void setMaxDistance(float maxDistance) override;
    virtual float getMaxDistance() const override;

    virtual void setVolume(float volume) override;
    virtual float getVolume() const override;

    virtual bool isFinished() const override;

    virtual size_t getLengthInMilliseconds() const override;
    virtual float getLengthInSeconds() const override;

    virtual size_t getPlayPositionInMilliseconds() const override;
    virtual void setPlayPositionInMilliseconds(uint32 position) override;
    virtual float getPlayPositionInSeconds() const override;
    virtual void setPlayPositionInSeconds(float position) override;
    virtual float getPlayPosition() const override;
    virtual void setPlayPosition(float position) override;

private:
    virtual void OnSoundStopped(irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData);

    const bool mIs3d;
    irrklang::ISound* mSound;
    RoVector3f mPosition;
};