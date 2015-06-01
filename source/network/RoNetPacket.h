////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_PACKET_H
#define ROKLEGEND_NET_PACKET_H

#include <core/RoPrerequisites.h>
#include <core/RoHashString.h>
#include <core/RoPropertyMap.h>

#include "RoNetTypes.h"

class RoNetPacket;
roDEFINE_PTR(RoNetPacket);

/**
  * Brief description about RoNetPacket.
 **/
class RoNetPacket
{
public:
    roINLINE RoNetPacket(const RoHashString& action, const RoHashDataMap& fields)
        : mAction(action)
        , mFields(fields)
    {
    }

    roINLINE virtual ~RoNetPacket()
    {
    }

    RoNetPacketPtr clone() const;

    bool equals(const RoNetPacket& other) const;

    roINLINE RoNetDataPtr at(size_t index)
    {
        return mFields[index];
    }
    roINLINE RoNetDataPtr at(size_t index) const
    {
        return mFields[index];
    }

    RoNetDataPtr getField(const RoHashString& name)
    {
        return mFields[name];
    }

    RoNetDataPtr getField(const RoHashString& name) const
    {
        return mFields[name];
    }

    size_t getNumberOfFields() const
    {
        return mFields.size();
    }

    roINLINE RoString getActionName() const
    {
        return mAction.str();
    }

    RoPropertyMap getProperties() const;

    void fromProperties(const RoPropertyMap& properties) const;

    void readFromStream(RoDataStream& stream);

    void writeToStream(RoDataStream& stream) const;

    void tryReadFromStream(RoDataStream& stream);
    void tryWriteToStream(RoDataStream& stream) const;

protected:
    RoNetPacket(const RoHashString& action)
        : mAction(action)
    {
    }

    const RoHashString  mAction;
    RoHashDataMap       mFields;
};

#endif // ROKLEGEND_NET_PACKET_H
