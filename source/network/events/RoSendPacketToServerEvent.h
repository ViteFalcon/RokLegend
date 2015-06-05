#pragma once
#include <core/task/RoTaskArgs.h>
#include "../packets/RoPacket.h"

roDEFINE_TASK_ARGS(RoSendPacketToServerEvent)
{
    RoPacketPtr packet;
};
