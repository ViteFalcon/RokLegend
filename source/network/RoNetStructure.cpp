////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoNetStructure.h"
#include "RoNetPacket.h"

RoNetStructure::RoNetStructure(const RoHashString& name, RoNetPacketPtr structure)
    : RoNetData(name)
    , mStructure(structure)
{
}

RoNetStructure::~RoNetStructure()
{
}

size_t RoNetStructure::size() const
{
    return 0;
}

RoNetDataPtr RoNetStructure::clone(RoNetPacket&) const
{
    return RoNetDataPtr(new RoNetStructure(getHash(), mStructure->clone()));
}

bool RoNetStructure::equalsImpl(const RoNetData& other) const
{
    const RoNetStructure* structPtr = dynamic_cast<const RoNetStructure*>(&other);
    if (structPtr && structPtr->mStructure && mStructure)
    {
        return structPtr->mStructure->equals(*mStructure);
    }
    return false;
}

RoVariant RoNetStructure::toVariant() const
{
    return RoVariant(mStructure);
}
