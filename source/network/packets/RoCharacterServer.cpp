#include "RoCharacterServer.h"

roREGISTER_SUB_PACKET(RoCharacterServer);

RoCharacterServer::RoCharacterServer()
{
    add<RoString>("ipaddress", &RoCharacterServer::mIpAddress);
    add<uint16>("port", &RoCharacterServer::mPort);
    add<RoString>("server_name", &RoCharacterServer::mServerName);
    add<uint16>("players", &RoCharacterServer::mPlayers);
    add<uint16>("new_data", &RoCharacterServer::mNewData);
}

RoServerType RoCharacterServer::getServerType() const
{
    switch (mServerType)
    {
    case 0:
        return RoServerType::NORMAL;
    case 1:
        return RoServerType::MAINTENANCE;
    case 2:
        return RoServerType::OVER_18;
    case 3:
        return RoServerType::PAID;
    case 4:
        return RoServerType::PAY_TO_PLAY;
    default:
        break;
    }
    return RoServerType::UNKNOWN;
}

