////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_BACKGROUND_MUSIC_H
#define ROKLEGEND_BACKGROUND_MUSIC_H

#include <core/RoPrerequisites.h>
#include <core/RoSharedPtr.h>

roFORWARD_DECL_PTR(RoAudio);
roFORWARD_DECL_PTR(RoBackgroundMusic);

/**
  * Class that plays background music.
 **/
class RoBackgroundMusic
{
public:
    static RoBackgroundMusicPtr NewBackgroundMusic(const RoString& filePath);

    virtual ~RoBackgroundMusic();

private:
    RoBackgroundMusic(const RoString& file);

private:
    const RoString mFile;
    RoAudioPtr mAudio;
};

#endif // ROKLEGEND_BACKGROUND_MUSIC_H
