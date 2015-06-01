////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_SERVER_STATUS_H
#define ROKLEGEND_NET_SERVER_STATUS_H

#include <core/RoPrerequisites.h>

/**
  * Enumeration for different types of server statuses.
 **/
enum RoNetServerStatus
{
    eNetServerStatus_Disconnected,
    eNetServerStatus_ConnectFailed,
    eNetServerStatus_Connected
};

RoString roNetServerStatusToString(RoNetServerStatus serverStatus);

#endif // ROKLEGEND_NET_SERVER_STATUS_H
