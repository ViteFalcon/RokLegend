#pragma once

#include "RoPrerequisites.h"
#include "RoHashString.h"

class RoDict
{
public:
    static const RoString GrfFile;
    static const RoString CameraName;
    static const RoString ConnectionIndex;

    static const RoHashString Action;
    static const RoHashString TimeSinceLastFrameInSecs;
    static const RoHashString PacketId;
    static const RoHashString ServerType;
    static const RoHashString Host;
    static const RoHashString PortNumber;
private:
    RoDict(void);
    ~RoDict(void);
};
