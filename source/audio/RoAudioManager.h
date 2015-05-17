////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <core/RoPrerequisites.h>
#include <core/RoConfig.h>
#include <math/RoVector3f.h>

roFORWARD_DECL_PTR(RoAudio);
roFORWARD_DECL_PTR(RoAudioManager);

/**
 * Audio manager.
 */
class RoAudioManager
{
public:
    static RoAudioManagerPtr Get(const RoConfig& config);
 
    RoAudioManager(const RoConfig& config);

    RoAudioManager(const RoAudioManager&) = delete;
    virtual ~RoAudioManager() = default;
 
    virtual RoAudioPtr getSound2D(const RoString& filename) = 0;
    virtual RoAudioPtr getSound3D(const RoString& filename) = 0;
    RoAudioPtr getBackgroundMusic(const RoString& filename);
 
    virtual void playSound2D(const RoString& filename, bool loop) = 0;
    virtual void playSound3D(const RoString& filename, bool loop, const RoVector3f& position) = 0;

private:
    const RoConfig mConfigs;
};

