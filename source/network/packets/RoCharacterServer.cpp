#include "RoCharacterServer.h"
#include <core/RoStringUtil.h>
#include <core/RoLog.h>

roREGISTER_SUB_PACKET(RoCharacterServer);

RoCharacterServer::RoCharacterServer()
{
    add<uint32>("ip_number", &RoCharacterServer::mIpNumber);
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
    roLOG_DBG << "Unknown server-type number: " << mServerType;
    return RoServerType::UNKNOWN;
}

std::string to_string(const RoServerType& serverType)
{
    switch (serverType)
    {
    case RoServerType::NORMAL:
        return "Normal Server";
    case RoServerType::MAINTENANCE:
        return "Maintenance Server";
    case RoServerType::OVER_18:
        return "Over 18 Server";
    case RoServerType::PAID:
        return "Paid Server";
    case RoServerType::PAY_TO_PLAY:
        return "Pay-to-Play Server";
    default:
        break;
    }
    return "Unknown Server Type";
}

/*
 * NOTE: In rAthena, IP address is in little-endian format
 */
#define roIP_W_DIVISOR 1
#define roIP_X_DIVISOR 256
#define roIP_Y_DIVISOR 65536
#define roIP_Z_DIVISOR 16777216

RoString RoCharacterServer::getIpAddress() const
{
    int w = int(mIpNumber / roIP_W_DIVISOR) % 256;
    int x = int(mIpNumber / roIP_X_DIVISOR) % 256;
    int y = int(mIpNumber / roIP_Y_DIVISOR) % 256;
    int z = int(mIpNumber / roIP_Z_DIVISOR) % 256;
    return RoStringUtil::Format(L"%d.%d.%d.%d", w, x, y, z);
}
