////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_PACKET_BUILDER_H
#define ROKLEGEND_NET_PACKET_BUILDER_H

#include <core/RoPrerequisites.h>
#include <core/RoIndexedMap.h>
#include <core/RoSharedPtr.h>

#include "RoNetTypes.h"

class RoNetPacket;
roDEFINE_PTR(RoNetPacket);

/**
  * Brief description about RoNetPacketBuilder.
 **/
class RoNetPacketBuilder
{
public:
    RoNetPacketBuilder(const RoHashString& action);
    virtual ~RoNetPacketBuilder();

    void addByte(const RoHashString& name, uint8 value = 0);
    void addBool(const RoHashString& name, bool value = false);
    void addUInt16(const RoHashString& name, uint16 value = 0);
    void addUInt32(const RoHashString& name, uint32 value = 0);
    void addString(const RoHashString& name, size_t size, const RoString& value = "");
    void addVariableString(const RoHashString& name, const RoHashString& dependentValue);
    void addList(const RoHashString& name, RoNetPacketPtr element, const RoHashString& dependentValue);
    void addStructure(const RoHashString& name, RoNetPacketPtr structure);

    RoNetPacketPtr getPacket() const;

private:
    RoNetPacketBuilder(const RoNetPacketBuilder&) = delete;
    RoNetPacketBuilder& operator = (const RoNetPacketBuilder&) = delete;

    void addField(RoNetData* data);

    const RoHashString  mAction;
    RoHashDataMap       mFields;
};

#endif // ROKLEGEND_NET_PACKET_BUILDER_H
