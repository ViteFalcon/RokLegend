////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoNetPacket.h"

#include <core/RoDict.h>
#include <core/RoException.h>
#include <core/RoLog.h>
#include <core/RoZipIterator.h>

#define roDEBUG_PACKET 0

#if defined(roDEBUG_PACKET) && roDEBUG_PACKET
#   define roPKT_LOG_DBG(msg) roLOG_DBG << msg
#else
#   define roPKT_LOG_DBG(msg)
#endif // roDEBUG_PACKET

RoNetPacketPtr RoNetPacket::clone() const
{
    RoNetPacketPtr myclone(new RoNetPacket(mAction));
    RoHashDataMap fields;
    std::for_each(mFields.begin(), mFields.end(), [&](const RoNetDataPtr& field) {
        fields.push_back(field->getHash(), field->clone(*myclone));
    });
    myclone->mFields = fields;
    return myclone;
}

bool RoNetPacket::equals(const RoNetPacket& other) const
{
    if (mAction != other.mAction || mFields.size() != other.mFields.size())
    {
        return false;
    }
    
    for (auto tuple : roZipIterator(mFields, other.mFields))
    {
        RoNetDataPtr field1, field2;
        boost::tie(field1, field2) = tuple;
        if (field1->equals(*field2))
        {
            return false;
        }
    }
    return true;
}

RoPropertyMap RoNetPacket::getProperties() const
{
    RoPropertyMap properties;
    std::for_each(mFields.begin(), mFields.end(), [&](const RoNetDataPtr& field) {
        if (RoDict::PacketId == field->getHash())
        {
            // Skip packet-ID
            return;
        }
        properties.set(field->toProperty());
    });
    return properties;
}

void RoNetPacket::fromProperties(const RoPropertyMap& properties) const
{
    // Skip packet-ID
    std::for_each(mFields.begin() + 1, mFields.end(), [&](const RoNetDataPtr& field) {
        RoHashString fieldName = field->getHash();
        if (properties.has(fieldName))
        {
            field->fromVariant(properties.get(fieldName));
        }
    });
}

void RoNetPacket::readFromStream(RoDataStream& stream)
{
    roPKT_LOG_DBG("Reading data for packet '" << mAction << "'...");
    std::for_each(mFields.begin(), mFields.end(), [&](RoNetDataPtr field) {
        // Skip packet-id as this would've already been read by network connection to identify this packet.
        if (RoDict::PacketId == field->getHash())
        {
            return;
        }
        roPKT_LOG_DBG("Reading field '" << field.getName() << "'...");
        field->readFromStream(stream);
        roPKT_LOG_DBG("Reading field '" << field.getName() << "'... [DONE]");
    });
    roPKT_LOG_DBG("Reading data for packet '" << mAction << "'... [DONE]");
}

void RoNetPacket::writeToStream(RoDataStream& stream) const
{
    roPKT_LOG_DBG("Writing data for packet '" << mAction << "'...");
    std::for_each(mFields.begin(), mFields.end(), [&](const RoNetDataPtr& field) {
        roPKT_LOG_DBG("Writing field '" << field->getName() << "'...");
        field->writeToStream(stream);
        roPKT_LOG_DBG("Writing field '" << field->getName() << "'... [DONE]");
    });
    roPKT_LOG_DBG("Writing data for packet '" << mAction << "'... [DONE]");
}

void RoNetPacket::tryReadFromStream(RoDataStream& stream)
{
    roTRY
        readFromStream(stream);
    roCATCH_STD_EXCEPTION
        roLOG_CRIT << "Reading data for packet '" << mAction << "'... [FAILED]";
        roEXIT_GAME;
    roCATCH_END
}

void RoNetPacket::tryWriteToStream(RoDataStream& stream) const
{
    roTRY
        writeToStream(stream);
    roCATCH_EXCEPTIONS_AND_EXIT
}
