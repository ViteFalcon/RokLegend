////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnar�k Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoTimer.h"
#include "RoPlatform.h"

#if roARCHITECTURE_IS_64
#   define roGetTickCount GetTickCount64
#else
#   define roGetTickCount GetTickCount
#endif

RoTimer::RoTimer()
{
    reset();
}

RoTimer::~RoTimer()
{
}

void RoTimer::reset()
{
    QueryPerformanceFrequency(&mFrequency);

    QueryPerformanceCounter(&mStartTime);

    mStartTick = roGetTickCount();

    mLastTime = 0;
}

uint64 RoTimer::getMilliseconds()
{
    LARGE_INTEGER curTime;

    QueryPerformanceCounter(&curTime);

    LONGLONG measuredTime = curTime.QuadPart - mStartTime.QuadPart;

    // scale by 1000 for milliseconds
    RoClockTick newTicks = (RoClockTick)(1000 * measuredTime / mFrequency.QuadPart);

    // detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    RoClockTick check = roGetTickCount() - mStartTick;
    LONGLONG msecOff = (LONGLONG)(newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = std::min<LONGLONG>(msecOff * mFrequency.QuadPart / 1000, measuredTime - mLastTime);
        mStartTime.QuadPart += adjust;
        measuredTime -= adjust;

        // Re-calculate milliseconds
        newTicks = (RoClockTick) (1000 * measuredTime / mFrequency.QuadPart);
    }
    // Record last time for adjust
    mLastTime = measuredTime;

    return newTicks;
}

uint64 RoTimer::getMicroseconds()
{
    return 10000 * getMilliseconds();
}

