////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_PACKET_DB_H
#define ROKLEGEND_NET_PACKET_DB_H

#include <core/RoPrerequisites.h>
#include <core/RoHashString.h>

#include "RoNetServerType.h"

class RoNetPacket;
roDEFINE_PTR(RoNetPacket);

class RoNetPacketDb;
roDEFINE_PTR(RoNetPacketDb);

/**
  * Brief description about RoNetPacketDb.
 **/
class RoNetPacketDb
{
public:
    static RoNetPacketDbPtr LoadFromSource(const RoString& source);

    virtual ~RoNetPacketDb();

    inline RoString getSource() const
    {
        return mSource;
    }

    virtual RoString getClientVersion(uint16 version) = 0;

    virtual bool hasPacket(uint16 packetId) = 0;

    virtual RoNetPacketPtr getPacketById(uint16 packetId) = 0;

    virtual RoNetPacketPtr getPacketByAction(const RoHashString& action) = 0;

    virtual RoNetPacketPtr getHeartBeatPacket(RoNetServerType serverType) = 0;

public: // Static consts
    static const RoHashString TypeByte;
    static const RoHashString TypeBool;
    static const RoHashString TypeUInt16;
    static const RoHashString TypeUInt32;
    static const RoHashString TypeString;
    static const RoHashString TypeList;
    static const RoHashString ConditionBytesRead;
    static const RoHashString ConditionRestOfPacket;

protected:
    RoNetPacketDb(const RoString& source);

    const RoString mSource;
};

#endif // ROKLEGEND_NET_PACKET_DB_H
