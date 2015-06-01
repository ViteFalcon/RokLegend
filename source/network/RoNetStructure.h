////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_STRUCTURE_H
#define ROKLEGEND_NET_STRUCTURE_H

#include <core/RoPrerequisites.h>

#include "RoNetData.h"

class RoNetPacket;
roDEFINE_PTR(RoNetPacket);

/**
  * Brief description about RoNetStructure.
 **/
class RoNetStructure : public RoNetData
{
public:
    RoNetStructure(const RoHashString& name, RoNetPacketPtr structure);
    virtual ~RoNetStructure();

    virtual size_t size() const;

    virtual RoNetDataPtr clone(RoNetPacket&) const;

    virtual RoVariant toVariant() const;
    virtual void fromVariant(const RoVariant& variant){}

    virtual RoString str() const
    {
        return "";
    }
private:
    virtual bool equalsImpl(const RoNetData& other) const;
    virtual void readImpl(RoDataStream& stream){}
    virtual void writeImpl(RoDataStream& stream) const{}

    RoNetPacketPtr mStructure;
};

#endif // ROKLEGEND_NET_STRUCTURE_H
