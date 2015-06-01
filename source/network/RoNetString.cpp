////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////



#include "RoNetString.h"
#include "RoNetPacket.h"

RoNetString::RoNetString(const RoHashString& name, RoNetStringContainerPtr container)
    : RoNetData(name)
    , mContainer(container)
{
}

RoNetString::~RoNetString()
{
}

RoNetDataPtr RoNetString::clone(RoNetPacket& packet) const
{
    return RoNetDataPtr(new RoNetString(getHash(), mContainer->clone(packet)));
}

RoVariant RoNetString::toVariant() const
{
    return RoVariant(mContainer->getString());
}

void RoNetString::fromVariant(const RoVariant& variant)
{
    std::string value;
    variant.as(value);
    mContainer->assign(value);
}

bool RoNetString::equalsImpl(const RoNetData& other) const
{
    const RoNetString* str = dynamic_cast<const RoNetString*>(&other);
    if (str)
    {
        return mContainer->getString() == str->mContainer->getString();
    }
    return false;
}

void RoNetString::readImpl(RoDataStream& stream)
{
    mContainer->readFromStream(stream);
}

void RoNetString::writeImpl(RoDataStream& stream) const
{
    mContainer->writeToStream(stream);
}

void RoNetString::assign(const RoString& val)
{
    mContainer->assign(val);
}
