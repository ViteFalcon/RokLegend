////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_UTILITIES_H
#define ROKLEGEND_UTILITIES_H

#include <Core/RoPrerequisites.h>

/**
  * Brief description about RoUtilities.
 **/
class RoUtilities
{
public:
    static RoString IpIntegerToString(uint32 integerIpAddress);

    static roINLINE void MemCopy(void* destination, size_t destinationSize, const void* source, size_t bytesToCopy)
    {
#if defined(_WIN32)
        memcpy_s(destination, destinationSize, source, bytesToCopy);
#else
        memcpy(destination, source, bytesToCopy);
#endif
    }

    template <typename T>
    static roINLINE T* NewSetToZero()
    {
        T* result = new T();
        memset(result, 0, sizeof(T));
        return result;
    }

private:
    RoUtilities();
    virtual ~RoUtilities();
};

#endif // ROKLEGEND_UTILITIES_H
