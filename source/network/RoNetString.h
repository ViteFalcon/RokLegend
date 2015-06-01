////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_STRING_H
#define ROKLEGEND_NET_STRING_H

#include <core/RoPrerequisites.h>
#include "RoNetTypes.h"
#include "RoNetStringContainer.h"

/**
  * Brief description about RoNetString.
 **/
class RoNetString : public RoNetData
{
public:
    RoNetString(const RoHashString& name, RoNetStringContainerPtr container);
    virtual ~RoNetString();

    virtual size_t size() const
    {
        return mContainer->getSize();
    }

    virtual RoNetDataPtr clone(RoNetPacket& packet) const;

    virtual RoVariant toVariant() const;
    virtual void fromVariant(const RoVariant& variant);

    virtual RoString toString() const
    {
        return mContainer->getString();
    }

protected:
    virtual bool equalsImpl(const RoNetData& other) const;
    virtual void readImpl(RoDataStream& stream);
    virtual void writeImpl(RoDataStream& stream) const;

    void assign(const RoString& val);

    const RoNetStringContainerPtr mContainer;
};

#endif // ROKLEGEND_NET_STRING_H
