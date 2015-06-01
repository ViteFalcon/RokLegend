////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoNetList.h"

RoNetList::RoNetList(RoHashString action, RoNetListContainerPtr container)
    : RoNetData(action)
    , mContainer(container)
{
}

RoNetList::~RoNetList()
{
}

RoNetDataPtr RoNetList::clone(RoNetPacket& packet) const
{
    return RoNetDataPtr(new RoNetList(getHash(), mContainer->clone(packet)));
}

RoVariant RoNetList::toVariant() const
{
    return RoVariant(mContainer->toPropertyMapArray());
}

void RoNetList::fromVariant(const RoVariant& variant)
{
    RoNetListContainer::ArrayType data;
    variant.as(data);
    mContainer->assign(data);
}

bool RoNetList::equalsImpl(const RoNetData& other) const
{
    const RoNetList* otherList = dynamic_cast<const RoNetList*>(&other);
    if (!otherList)
    {
        return false;
    }
    return mContainer->equals(*(otherList->mContainer));
}

void RoNetList::readImpl(RoDataStream& stream)
{
    mContainer->readFromStream(stream);
}

void RoNetList::writeImpl(RoDataStream& stream) const
{
    mContainer->writeToStream(stream);
}
