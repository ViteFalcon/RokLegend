////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_LIST_H
#define ROKLEGEND_NET_LIST_H

#include "RoNetData.h"
#include "RoNetListContainer.h"

/**
  * Brief description about RoNetList.
 **/
class RoNetList : public RoNetData
{
public:
    RoNetList(RoHashString action, RoNetListContainerPtr container);
    virtual ~RoNetList();

    virtual size_t size() const
    {
        return mContainer->getSize();
    }

    virtual RoNetDataPtr clone(RoNetPacket& packet) const;

    virtual RoVariant toVariant() const;
    virtual void fromVariant(const RoVariant& variant);

    virtual RoString toString() const
    {
        return mContainer->toString();
    }

private:
    virtual bool equalsImpl(const RoNetData& other) const;
    virtual void readImpl(RoDataStream& stream);
    virtual void writeImpl(RoDataStream& stream) const;

    RoNetListContainerPtr mContainer;
};

#endif // ROKLEGEND_NET_LIST_H
