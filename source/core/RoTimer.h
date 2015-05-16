////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROGRF_TIMER_H
#define ROGRF_TIMER_H

#include "RoPrerequisites.h"

/**
  * High resolution timer.
 **/
class RoTimer
{
public:
    RoTimer();
    virtual ~RoTimer();

    /** Resets timer */
    void reset();

    /** Returns milliseconds since initialisation or last reset */
    uint64 getMilliseconds();

    /** Returns microseconds since initialisation or last reset */
    uint64 getMicroseconds();

private:
    DWORD mStartTick;
    LONGLONG mLastTime;
    LARGE_INTEGER mStartTime;
    LARGE_INTEGER mFrequency;
};

#endif // ROGRF_TIMER_H
