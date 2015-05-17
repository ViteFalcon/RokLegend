////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2015
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <core/RoPrerequisites.h>
#include <core/RoSharedPtr.h>
#include <math/RoVector3f.h>

class RoAudio
{
public:
    virtual ~RoAudio() = default;

    virtual bool is3d() const = 0;

    virtual bool isEmpty() const = 0;

    virtual void setIsLooped(bool looped) = 0;
    virtual bool isLooped() const = 0;

    virtual void setIsPaused(bool paused) = 0;
    virtual bool isPaused() const = 0;

    virtual void setPosition(const RoVector3f& position) = 0;
    virtual RoVector3f getPosition() const = 0;

    virtual void setMinDistance(float minDistance) = 0;
    virtual float getMinDistance() const = 0;

    virtual void setMaxDistance(float maxDistance) = 0;
    virtual float getMaxDistance() const = 0;

    virtual void setVolume(float volume) = 0;
    virtual float getVolume() const = 0;

    virtual bool isFinished() const = 0;

    virtual size_t getLengthInMilliseconds() const = 0;
    virtual float getLengthInSeconds() const = 0;

    virtual size_t getPlayPositionInMilliseconds() const = 0;
    virtual void setPlayPositionInMilliseconds(uint32 position) = 0;
    virtual float getPlayPositionInSeconds() const = 0;
    virtual void setPlayPositionInSeconds(float position) = 0;
    virtual float getPlayPosition() const = 0;
    virtual void setPlayPosition(float position) = 0;
};
