#pragma once
#include <core/task/RoTaskArgs.h>

roDEFINE_TASK_ARGS(RoServerConnectRequestEvent)
{
    RoNetServerType serverType;
    RoString ipAddress;
    RoString portNumber;
};