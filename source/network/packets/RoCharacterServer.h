#pragma once

#include "RoPacket.h"

enum class RoServerType
{
    UNKNOWN,
    NORMAL,
    MAINTENANCE,
    OVER_18,
    PAID,
    PAY_TO_PLAY
};

std::string to_string(const RoServerType& serverType);

roDEFINE_PACKET(RoCharacterServer)
{
public:
    RoCharacterServer();

    RoString getIpAddress() const
    {
        return mIpAddress;
    }

    uint16 getPortNumber() const
    {
        return mPort;
    }

    RoString getName() const
    {
        return mServerName;
    }

    uint16 getNumberOfPlayers() const
    {
        return mPlayers;
    }

    RoServerType getServerType() const;
private:
    RoString mIpAddress;
    uint16 mPort;
    RoString mServerName;
    uint16 mPlayers;
    uint16 mServerType;
    uint16 mNewData;
};

roDEFINE_PTR(RoCharacterServer);
