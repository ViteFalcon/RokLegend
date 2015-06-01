#pragma once

#include <core/task/RoTaskArgs.h>
#include "../RoNetServerType.h"

roDEFINE_TASK_ARGS(RoServerConnectRequestFailedEvent)
{
    RoNetServerType serverType;
    RoString reason;
};