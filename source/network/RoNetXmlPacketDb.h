////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_XML_PACKET_DB_H
#define ROKLEGEND_NET_XML_PACKET_DB_H

#include <core/RoPrerequisites.h>

#include "RoNetPacketDb.h"

#include <boost/unordered_map.hpp>

namespace tinyxml2
{
    class XMLElement;
}

class RoNetPacketBuilder;
class RoXmlAttributeReader;

/**
  * Packet DB that reads packet format from XML.
 **/
class RoNetXmlPacketDb : public RoNetPacketDb
{
public:
    static bool CanReadDbSource(const RoString& source);

    RoNetXmlPacketDb(const RoString& xmlFilePath);
    virtual ~RoNetXmlPacketDb();

    virtual RoString getClientVersion(uint16 version);

    virtual bool hasPacket(uint16 packetId);

    virtual RoNetPacketPtr getPacketById(uint16 packetId);

    virtual RoNetPacketPtr getPacketByAction(const RoHashString& action);

    virtual RoNetPacketPtr getHeartBeatPacket(RoNetServerType serverType);

    inline bool hasStructure(RoHashString name) const
    {
        return 0 != mStructures.count(name);
    }

    RoNetPacketPtr getStructure(RoHashString name) const;

private: // functions
    void loadPacketDatabase(const RoString& xmlFilePath);
    void parseVersions(tinyxml2::XMLElement* versions);
    bool hasPacketVersion(uint32 packetVersion) const;
    void parseStructures(tinyxml2::XMLElement* structures);
    void parseHeartBeatPackets(tinyxml2::XMLElement* heartBeatPackets);
    void parsePackets(tinyxml2::XMLElement* packets);
    void parseAllData(tinyxml2::XMLElement* data, RoNetPacketBuilder& packetBuilder);
private: // data
    typedef boost::unordered_map<RoNetServerType, RoNetPacketPtr> HeartBeatPacketMap;
    typedef boost::unordered_map<RoHashString, RoNetPacketPtr> StructMap;
    typedef boost::unordered_map<uint16, RoNetPacketPtr> PacketMap;
    typedef boost::unordered_map<RoHashString, uint16> ActionPacketIdMap;
    typedef boost::unordered_map<uint16, RoString> VersionMap;
    typedef boost::function<void (RoNetXmlPacketDb&, RoXmlAttributeReader&, RoNetPacketBuilder&)> XmlDataParser;
    typedef boost::unordered_map<RoHashString, XmlDataParser> XmlParserMap;

    static const XmlParserMap sXmlDataParsers;

    HeartBeatPacketMap mHeartBeatPackets;
    StructMap mStructures;
    PacketMap mPackets;
    ActionPacketIdMap mActionToPacketId;
    VersionMap mVersions;
};

#endif // ROKLEGEND_NET_XML_PACKET_DB_H
