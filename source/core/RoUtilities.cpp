////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoUtilities.h"
#if roPLATFORM_IS_WIN32
#   include <inaddr.h>
#   include <WinSock.h>
#elif roPLATFORM_IS_LINUX
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#endif // roPLATFORM_IS_WIN32

RoUtilities::RoUtilities()
{
}

RoUtilities::~RoUtilities()
{
}
//------------------------------------------------------------------------------
RoString RoUtilities::IpIntegerToString(uint32 integerIpAddress)
{
    struct in_addr ipAddress;
#if roPLATFORM_IS_WIN32
    ipAddress.S_un.S_addr = integerIpAddress;
#elif roPLATFORM_IS_LINUX
    ipAddress.s_addr = integerIpAddress;
#endif // roPLATFORM_IS_WIN32
    return RoString(inet_ntoa(ipAddress));
}
