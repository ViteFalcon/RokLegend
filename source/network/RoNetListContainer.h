////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_LIST_CONTAINER_H
#define ROKLEGEND_NET_LIST_CONTAINER_H

#include <core/RoPrerequisites.h>
#include <core/RoPropertyMap.h>
#include <core/RoDataStream.h>

#include "RoNetData.h"

class RoNetListContainer;
roDEFINE_PTR(RoNetListContainer);

class RoNetPacket;
roDEFINE_PTR(RoNetPacket);

/**
  * Read/write policy for lists in packets.
 **/
class RoNetListContainer
{
public:
    using ArrayType = std::vector<RoNetPacketPtr>;
    using Iterator = ArrayType::iterator;
    using ConstIterator = ArrayType::const_iterator;

    virtual ~RoNetListContainer();

    virtual RoNetListContainerPtr clone(RoNetPacket& packet) const = 0;

    virtual void readFromStream(RoDataStream& stream) = 0;
    virtual void writeToStream(RoDataStream& stream) const = 0;

    size_t getSize() const
    {
        return mDataArray.size();
    }

    RoNetPacketPtr at(size_t index) const
    {
        return mDataArray[index];
    }

    Iterator begin()
    {
        return mDataArray.begin();
    }

    Iterator end()
    {
        return mDataArray.end();
    }

    ConstIterator begin() const
    {
        return mDataArray.begin();
    }

    ConstIterator end() const
    {
        return mDataArray.end();
    }

    RoString toString() const;

    RoPropertyMapArray toPropertyMapArray() const;

    roINLINE void assign(const ArrayType& data)
    {
        mDataArray = data;
    }

    bool equals(const RoNetListContainer& other) const;

protected:
    RoNetListContainer(RoNetPacketPtr elementFormat);

    const RoNetPacketPtr mElement;
    ArrayType mDataArray;
};


class RoNetListRestOfPacketContainer : public RoNetListContainer
{
public:
    RoNetListRestOfPacketContainer(RoNetPacketPtr elementFormat, RoNetDataPtr packetSizeField);

    virtual RoNetListContainerPtr clone(RoNetPacket& packet) const;

    virtual void readFromStream(RoDataStream& stream);
    virtual void writeToStream(RoDataStream& stream) const;

private:
    RoNetDataPtr mPacketSizeField;
};

#endif // ROKLEGEND_NET_LIST_CONTAINER_H
