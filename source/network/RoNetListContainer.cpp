////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoNetListContainer.h"
#include "RoNetPacket.h"
#include "RoNetTypes.h"

#include <core/RoZipIterator.h>

RoNetListContainer::RoNetListContainer(RoNetPacketPtr elementFormat)
    : mElement(elementFormat)
{
}

RoNetListContainer::~RoNetListContainer()
{
}

RoString RoNetListContainer::toString() const
{
    RoStringBuilder stringBuilder;
    size_t index = 0;
    std::for_each(begin(), end(), [&](const RoNetPacketPtr& element) {
        stringBuilder << element->getActionName() << "[" << index++ << "]" << std::endl;
        stringBuilder.increaseIndentation();
        size_t numberOfFields = element->getNumberOfFields();
        for (size_t i = 0; i < numberOfFields; ++i)
        {
            element->at(i)->toString(stringBuilder);
        }
        stringBuilder.decreaseIndentation();
    });
    return stringBuilder.toString();
}

RoPropertyMapArray RoNetListContainer::toPropertyMapArray() const
{
    RoPropertyMapArray propertyArray;
    if (mDataArray.empty())
    {
        return propertyArray;
    }
    propertyArray.reserve(mDataArray.size());
    std::for_each(begin(), end(), [&](const RoNetPacketPtr& element) {
        propertyArray.push_back(element->getProperties());
    });
    return propertyArray;
}

bool RoNetListContainer::equals(const RoNetListContainer& other) const
{
    if (mDataArray.size() != other.mDataArray.size())
    {
        return false;
    }

    for (auto tuple : roZipIterator(*this, other))
    {
        RoNetPacketPtr myElement, theirElement;
        boost::tie(myElement, theirElement) = tuple;
        if (!myElement->equals(*theirElement))
        {
            return false;
        }
    }
    return true;
}

RoNetListRestOfPacketContainer::RoNetListRestOfPacketContainer(RoNetPacketPtr elementFormat, RoNetDataPtr packetSizeField)
    : RoNetListContainer(elementFormat)
    , mPacketSizeField(packetSizeField)
{
}

RoNetListContainerPtr RoNetListRestOfPacketContainer::clone(RoNetPacket& packet) const
{
    return RoNetListContainerPtr(new RoNetListRestOfPacketContainer(mElement->clone(), packet.getField(mPacketSizeField->getHash())));
}

void RoNetListRestOfPacketContainer::readFromStream(RoDataStream& stream)
{
    const uint32 packetSize = mPacketSizeField->as<uint32>();
    std::list<RoNetPacketPtr> elements;
    while (stream.bytesRead() < packetSize)
    {
        RoNetPacketPtr element = mElement->clone();
        element->readFromStream(stream);
        elements.push_back(element);
    }
    mDataArray.assign(elements.begin(), elements.end());
}

void RoNetListRestOfPacketContainer::writeToStream(RoDataStream& stream) const
{
    std::for_each(begin(), end(), [&](const RoNetPacketPtr& element) {
        element->writeToStream(stream);
    });
    uint32 streamPos = uint32(stream.tell());
    mPacketSizeField->fromVariant(RoVariant(streamPos));
    RoNetData::RewriteDataToStream(mPacketSizeField, stream);
}
