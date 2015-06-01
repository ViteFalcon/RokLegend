////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoNetPacketBuilder.h"
#include "RoNetPacket.h"
#include "RoNetList.h"

#include <core/RoException.h>
#include <core/RoErrorInfo.h>

RoNetPacketBuilder::RoNetPacketBuilder(const RoHashString& action)
    : mAction(action)
{
}

RoNetPacketBuilder::~RoNetPacketBuilder()
{
}

void RoNetPacketBuilder::addByte(const RoHashString& name, uint8 value)
{
    addField(new RoNetUInt8(name, value));
}

void RoNetPacketBuilder::addBool(const RoHashString& name, bool value)
{
    addField(new RoNetBool(name, value));
}

void RoNetPacketBuilder::addUInt16(const RoHashString& name, uint16 value)
{
    addField(new RoNetUInt16(name, value));
}

void RoNetPacketBuilder::addUInt32(const RoHashString& name, uint32 value)
{
    addField(new RoNetUInt32(name, value));
}

void RoNetPacketBuilder::addString(const RoHashString& name, size_t size, const RoString& value)
{
    RoNetStringContainerPtr fixedSizeContainer(new RoNetStringFixedSizeContainer(size, value));
    addField(new RoNetString(name, fixedSizeContainer));
}

void RoNetPacketBuilder::addVariableString(const RoHashString& name, const RoHashString& dependentValue)
{
    RoNetStringContainerPtr nullTerminatedContainer(new RoNetStringNullTerminatedContainer(""));
    addField(new RoNetString(name, nullTerminatedContainer));
}

void RoNetPacketBuilder::addList(const RoHashString& name, RoNetPacketPtr element, const RoHashString& dependentValue)
{
    if(!mFields.has(dependentValue))
    {
        roTHROW(RoItemNotFound()
            << RoErrorInfoDetail("Could not find dependent field for list.")
            << RoErrorInfoItemName(dependentValue.str()));
    }
    RoNetListContainerPtr listContainer(new RoNetListRestOfPacketContainer(element, mFields[dependentValue]));
    addField(new RoNetList(name, listContainer));
}

void RoNetPacketBuilder::addStructure(const RoHashString& name, RoNetPacketPtr structure)
{
}

RoNetPacketPtr RoNetPacketBuilder::getPacket() const
{
    return RoNetPacketPtr(new RoNetPacket(mAction, mFields));
}

void RoNetPacketBuilder::addField(RoNetData* field)
{
    mFields.push_back(field->getHash(), RoNetDataPtr(field));
}
