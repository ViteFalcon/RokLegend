////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoTimer.h"
#include "RoPlatform.h"

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
    mStartTick = GetTickCount();

    mLastTime = 0;
}

uint64 RoTimer::getMilliseconds()
{
    LARGE_INTEGER curTime;

    QueryPerformanceCounter(&curTime);

    LONGLONG measuredTime = curTime.QuadPart - mStartTime.QuadPart;

    // scale by 1000 for milliseconds
    unsigned long newTicks = (unsigned long) (1000 * measuredTime / mFrequency.QuadPart);

    // detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    unsigned long check = GetTickCount() - mStartTick;
    signed long msecOff = (signed long)(newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, measuredTime - mLastTime);
        mStartTime.QuadPart += adjust;
        measuredTime -= adjust;

        // Re-calculate milliseconds
        newTicks = (unsigned long) (1000 * measuredTime / mFrequency.QuadPart);
    }
    // Record last time for adjust
    mLastTime = measuredTime;

    return newTicks;
}

uint64 RoTimer::getMicroseconds()
{
    LARGE_INTEGER curTime;

    QueryPerformanceCounter(&curTime);

    LONGLONG measuredTime = curTime.QuadPart - mStartTime.QuadPart;

    // scale by 1000 for milliseconds
    unsigned long newTicks = (unsigned long) (1000 * measuredTime / mFrequency.QuadPart);

    // detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    unsigned long check = GetTickCount() - mStartTick;
    signed long msecOff = (signed long)(newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, measuredTime - mLastTime);
        mStartTime.QuadPart += adjust;
        measuredTime -= adjust;
    }
    // Record last time for adjust
    mLastTime = measuredTime;

    return uint64(10000 * measuredTime / mFrequency.QuadPart);
}

