#pragma once
#include <core/task/RoTaskArgs.h>

roDEFINE_TASK_ARGS(RoServerConnectRequestFailedEvent)
{
    RoNetServerType serverType;
    RoString reason;
};