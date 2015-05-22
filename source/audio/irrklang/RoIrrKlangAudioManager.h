////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../RoAudioManager.h"
#include <irrKlang.h>

/**
  * Audio manager.
 **/
class RoIrrKlangAudioManager : public RoAudioManager
{
public:
    RoIrrKlangAudioManager(const RoConfig& config);
    virtual ~RoIrrKlangAudioManager();

    virtual RoAudioPtr getSound2D(const RoString& filename) override;
    virtual RoAudioPtr getSound2D(const RoString& filename, const RoDataStreamPtr& data, bool loop) override;
    virtual RoAudioPtr getSound3D(const RoString& filename) override;

    virtual void playSound2D(const RoString& filename, bool loop) override;
    virtual void playSound3D(const RoString& filename, bool loop, const RoVector3f& position) override;

private:
    irrklang::ISoundEngine* mEngine;
};
