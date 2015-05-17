////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
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

    /** Returns milliseconds since initialization or last reset */
    uint64 getMilliseconds();

    /** Returns microseconds since initialization or last reset */
    uint64 getMicroseconds();

private:
#if roARCHITECTURE_IS_64
    using RoClockTick = ULONGLONG;
#else
    using RoClockTick = DWORD;
#endif
    RoClockTick mStartTick;
    ULONGLONG mLastTime;
    LARGE_INTEGER mStartTime;
    LARGE_INTEGER mFrequency;
};

#endif // ROGRF_TIMER_H
