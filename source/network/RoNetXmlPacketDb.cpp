////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////


#include "RoNetXmlPacketDb.h"
#include "RoNetPacket.h"
#include "RoNetPacketBuilder.h"

#include <core/RoDict.h>
#include <core/RoException.h>
#include <core/RoErrorInfo.h>
#include <core/RoFileSystem.h>
#include <core/RoStringUtil.h>
#include <core/RoLog.h>

#include <xml/tinyxml/tinyxml2.h>
#include <xml/RoXmlAttributeReader.h>
#include <xml/RoXmlDocumentErrorHandler.h>
#include <xml/RoXmlElementIterator.h>

#include <boost/utility/value_init.hpp>

roDEFINE_EXCEPTION(XmlPacketDbError);
roDEFINE_EXCEPTION(XmlPacketDbTinyXMLError);
//=============================================================================
namespace {
#   define roMISSING_DATA_ATTRIBUTE_MSG(type, name) "A '"type"' data entry should specify the '"name"' of the data."

    void roParseXmlBool(RoNetXmlPacketDb& packetDb, RoXmlAttributeReader& dataAttributes, RoNetPacketBuilder& packetBuilder)
    {
        RoHashString dataName = dataAttributes.getHashString("name", roMISSING_DATA_ATTRIBUTE_MSG("bool", "name"));
        bool dataValue = dataAttributes.tryToGetBool("value", boost::initialized_value);
        packetBuilder.addBool(dataName, dataValue);
    }

    void roParseXmlByte(RoNetXmlPacketDb& packetDb, RoXmlAttributeReader& dataAttributes, RoNetPacketBuilder& packetBuilder)
    {
        RoHashString dataName = dataAttributes.getHashString("name", roMISSING_DATA_ATTRIBUTE_MSG("byte", "name"));
        uint8 dataValue = dataAttributes.tryToGetByte("value", boost::initialized_value);
        packetBuilder.addByte(dataName, dataValue);
    }

    void roParseXmlUInt16(RoNetXmlPacketDb& packetDb, RoXmlAttributeReader& dataAttributes, RoNetPacketBuilder& packetBuilder)
    {
        RoHashString dataName = dataAttributes.getHashString("name", roMISSING_DATA_ATTRIBUTE_MSG("uint16", "name"));
        uint16 dataValue = dataAttributes.tryToGetUInt16("value", boost::initialized_value);
        packetBuilder.addUInt16(dataName, dataValue);
    }

    void roParseXmlUInt32(RoNetXmlPacketDb& packetDb, RoXmlAttributeReader& dataAttributes, RoNetPacketBuilder& packetBuilder)
    {
        RoHashString dataName = dataAttributes.getHashString("name", roMISSING_DATA_ATTRIBUTE_MSG("uint32", "name"));
        uint32 dataValue = dataAttributes.tryToGetUInt32("value", boost::initialized_value);
        packetBuilder.addUInt32(dataName, dataValue);
    }

    void roParseXmlFixedString(RoXmlAttributeReader& dataAttributes, RoNetPacketBuilder& packetBuilder)
    {
        if (dataAttributes.has("dependson"))
        {
            roTHROW(RoXmlPacketDbError()
                << RoErrorInfoDetail("A fixed-size string shouldn't define a 'dependon' attribute.")
                << RoErrorInfoHint("A string can either be fixed-size (with 'size' attribute defined) or variable-sized, not both."));
        }
        RoHashString dataName = dataAttributes.getHashString("name", roMISSING_DATA_ATTRIBUTE_MSG("fixed-size string", "name"));
        uint32 dataSize = dataAttributes.getUInt32("size", roMISSING_DATA_ATTRIBUTE_MSG("fixed-size string", "size"));
        RoString dataValue = dataAttributes.tryToGetString("value", boost::initialized_value);
        packetBuilder.addString(dataName, dataSize, dataValue);
    }

    void roParseXmlVariableString(RoXmlAttributeReader& dataAttributes, RoNetPacketBuilder& packetBuilder)
    {
        if (dataAttributes.has("size"))
        {
            roTHROW(RoXmlPacketDbError()
                << RoErrorInfoDetail("A variable-sized string shouldn't define a 'size' attribute.")
                << RoErrorInfoHint("A string can either be fixed-size or variable-sized (with 'dependson' attribute defined), not both."));
        }
        RoHashString dataName = dataAttributes.getHashString("name", roMISSING_DATA_ATTRIBUTE_MSG("variable-length string", "name"));
        RoHashString dataDependsOn = dataAttributes.getHashString("dependson", roMISSING_DATA_ATTRIBUTE_MSG("variable-length string", "dependson"));
        packetBuilder.addVariableString(dataName, dataDependsOn);
    }

    void roParseXmlString(RoNetXmlPacketDb& packetDb, RoXmlAttributeReader& dataAttributes, RoNetPacketBuilder& packetBuilder)
    {
        if (dataAttributes.has("size"))
        {
            roParseXmlFixedString(dataAttributes, packetBuilder);
        }
        else
        {
            roParseXmlVariableString(dataAttributes, packetBuilder);
        }
    }

    void roParseXmlList(RoNetXmlPacketDb& packetDb, RoXmlAttributeReader& dataAttributes, RoNetPacketBuilder& packetBuilder)
    {
        RoHashString listName = dataAttributes.getHashString("name", roMISSING_DATA_ATTRIBUTE_MSG("list", "name"));
        RoHashString elementType = dataAttributes.getHashString("element", roMISSING_DATA_ATTRIBUTE_MSG("list", "type"));
        if (!packetDb.hasStructure(elementType))
        {
            roTHROW(RoXmlPacketDbError()
                << RoErrorInfoDetail("The element-type of a list should be a pre-defined structure.")
                << RoErrorInfoItemName(listName.str()));
        }
        RoNetPacketPtr element = packetDb.getStructure(elementType);
        RoHashString dependsOn = dataAttributes.getHashString("dependson", roMISSING_DATA_ATTRIBUTE_MSG("list", "dependson"));
        packetBuilder.addList(listName, element, dependsOn);
    }
}
//=============================================================================
const RoNetXmlPacketDb::XmlParserMap RoNetXmlPacketDb::sXmlDataParsers = {
    { RoNetPacketDb::TypeBool, &roParseXmlBool },
    { RoNetPacketDb::TypeByte, &roParseXmlByte },
    { RoNetPacketDb::TypeUInt16, &roParseXmlUInt16 },
    { RoNetPacketDb::TypeUInt32, &roParseXmlUInt32 },
    { RoNetPacketDb::TypeString, &roParseXmlString },
    { RoNetPacketDb::TypeList, &roParseXmlList }
};
//-----------------------------------------------------------------------------
bool RoNetXmlPacketDb::CanReadDbSource(const RoString& source)
{
    if (RoFileSystem::FileExists(source.asWStr()) && RoFileSystem::FileHasExtension(source.asWStr(), ".xml"))
    {
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
RoNetXmlPacketDb::RoNetXmlPacketDb(const RoString& xmlFilePath)
    : RoNetPacketDb(xmlFilePath)
{
    loadPacketDatabase(xmlFilePath);
}
//-----------------------------------------------------------------------------
RoNetXmlPacketDb::~RoNetXmlPacketDb()
{
}
//-----------------------------------------------------------------------------
RoString RoNetXmlPacketDb::getClientVersion(uint16 version)
{
    VersionMap::const_iterator versionItr = mVersions.find(version);
    if (mVersions.end() == versionItr)
    {
        return "Unknown";
    }
    return versionItr->second;
}
//-----------------------------------------------------------------------------
bool RoNetXmlPacketDb::hasPacket(uint16 packetId)
{
    return 0 != mPackets.count(packetId);
}
//-----------------------------------------------------------------------------
RoNetPacketPtr RoNetXmlPacketDb::getPacketById(uint16 packetId)
{
    RoNetPacketPtr packet;
    PacketMap::const_iterator packetItr = mPackets.find(packetId);
    if (mPackets.end() != packetItr)
    {
        packet = packetItr->second;
    }
    return packet;
}
//-----------------------------------------------------------------------------
RoNetPacketPtr RoNetXmlPacketDb::getPacketByAction(const RoHashString& action)
{
    RoNetPacketPtr packet;
    ActionPacketIdMap::const_iterator actionToIdIterator = mActionToPacketId.find(action);
    if (mActionToPacketId.end() != actionToIdIterator)
    {
        packet = mPackets[actionToIdIterator->second];
    }
    return packet;
}
//-----------------------------------------------------------------------------
RoNetPacketPtr RoNetXmlPacketDb::getHeartBeatPacket(RoNetServerType serverType)
{
    RoNetPacketPtr heartBeatPacket;
    HeartBeatPacketMap::const_iterator heartBeatPacketIterator = mHeartBeatPackets.find(serverType);
    if (mHeartBeatPackets.end() != heartBeatPacketIterator)
    {
        heartBeatPacket = heartBeatPacketIterator->second;
    }
    return heartBeatPacket;
}
//-----------------------------------------------------------------------------
roINLINE RoNetPacketPtr RoNetXmlPacketDb::getStructure(RoHashString name) const
{
    StructMap::const_iterator structureIterator = mStructures.find(name);
    if (mStructures.end() == structureIterator)
    {
        roTHROW(RoXmlPacketDbError()
            << RoErrorInfoDetail("A structure of requested doesn't exist in this packet database.")
            << RoErrorInfoItemName(name.str()));
    }
    return structureIterator->second;
}
//-----------------------------------------------------------------------------
roINLINE void RoNetXmlPacketDb::loadPacketDatabase(const RoString& xmlFilePath)
{
    tinyxml2::XMLDocument packetDb;
    RoXmlDocumentErrorHandler errorHandler(packetDb);
    errorHandler = packetDb.LoadFile(RoString::ToCurrentEncoding(xmlFilePath).c_str());
    tinyxml2::XMLElement* root = packetDb.RootElement();
    if (!boost::iequals("packets", root->Value()))
    {
        roTHROW(RoXmlPacketDbError()
            << RoErrorInfoDetail("The given packet XML database file seems to be invalid.")
            << RoErrorInfoHint("The root element of the XML database file should be 'packets'."));
    }
    parseVersions(root->FirstChildElement("versions"));
    parseStructures(root->FirstChildElement("structures"));
    parseHeartBeatPackets(root->FirstChildElement("heartbeatpackets"));
    parsePackets(root);
}
//-----------------------------------------------------------------------------
roINLINE void RoNetXmlPacketDb::parseVersions(tinyxml2::XMLElement* versions)
{
    if (!versions)
    {
        roLOG_WARN << "No version definitions found!";
        return;
    }
    RoXmlElementIterator versionXmlIterator(versions, "version");
    while (versionXmlIterator.hasMore())
    {
        tinyxml2::XMLElement* versionElement = versionXmlIterator.getNext();
        RoXmlAttributeReader attributeReader(versionElement);
        uint32 version = attributeReader.getUInt32("id", "Version entries should define 'id' as an unsigned-integer.");
        RoString client = attributeReader.getString("client", "Version entries need to specify 'client' atribute that specifies the client's version.");
        mVersions[version] = client;
    }
}
//-----------------------------------------------------------------------------
roINLINE bool RoNetXmlPacketDb::hasPacketVersion(uint32 packetVersion) const
{
    return 0 != mVersions.count(packetVersion);
}
//-----------------------------------------------------------------------------
roINLINE void RoNetXmlPacketDb::parseStructures(tinyxml2::XMLElement* structures)
{
    if (!structures)
    {
        roLOG_WARN << "No structure definitions found!";
        return;
    }
    RoXmlElementIterator structureXmlIterator(structures, "struct");
    while (structureXmlIterator.hasMore())
    {
        tinyxml2::XMLElement* structureElement = structureXmlIterator.getNext();
        RoXmlAttributeReader structureAttributes(structureElement);
        RoHashString structureName = structureAttributes.getHashString("name", "A structure should specify a name for reference!");
        uint32 structurePacketVersion = structureAttributes.getUInt32("version", "The structure named '' is missing its version number.");
        if (!hasPacketVersion(structurePacketVersion))
        {
            roTHROW(RoXmlPacketDbError()
                << RoErrorInfoDetail("Invalid packet version number defined for structure.")
                << RoErrorInfoItemName(structureName.str()));
        }

        if (roCLIENT_VERSION >= structurePacketVersion)
        {
            RoNetPacketBuilder structureBuilder(structureName);
            parseAllData(structureElement, structureBuilder);
            mStructures[structureName] = structureBuilder.getPacket();
        }
    }
}
//-----------------------------------------------------------------------------
roINLINE void RoNetXmlPacketDb::parseHeartBeatPackets(tinyxml2::XMLElement* heartBeatPackets)
{
    if (!heartBeatPackets)
    {
        roLOG_WARN << "No heart-beat packet definitions found!";
        return;
    }
    RoXmlElementIterator heartBeatElementIterator(heartBeatPackets, "heartbeatpacket");
    while (heartBeatElementIterator.hasMore())
    {
        tinyxml2::XMLElement* heartBeatPacketElement = heartBeatElementIterator.getNext();
        RoXmlAttributeReader heartBeatPacketAttributes(heartBeatPacketElement);
        uint16 heartBeatId = heartBeatPacketAttributes.getUInt16("id", "A heart-beat packet definition should specify a unique 16-bit integer packet 'id'.");
        RoString heartBeatServer = heartBeatPacketAttributes.getString("server", "A heart-beat packet definition should specify a server-name.");
        RoNetServerType heartBeatServerType = roNetServerTypeFromString(heartBeatServer);
        if (RoNetServerType::NONE == heartBeatServerType)
        {
            roTHROW(RoXmlPacketDbError()
                << RoErrorInfoDetail("Invalid server-type specified for heart-beat packet.")
                << RoErrorInfoItemName(heartBeatServer));
        }
        uint32 version = heartBeatPacketAttributes.getUInt32("version", "A heart-beat packet definition should specify a packet version.");
        if (!hasPacketVersion(version))
        {
            roTHROW(RoXmlPacketDbError()
                << RoErrorInfoDetail("Invalid version number specified for heart-beat packet.")
                << RoErrorInfoItemName(RoStringUtil::Format("Unknown Version: %d", version)));
        }
        if (roCLIENT_VERSION >= version)
        {
            std::stringstream heartBeatAction;
            heartBeatAction << heartBeatServer << ".HeartBeat";
            RoNetPacketBuilder heartBeatPacketBuilder(RoHashString(heartBeatAction.str().c_str()));
            heartBeatPacketBuilder.addUInt16(RoDict::PacketId, heartBeatId);
            parseAllData(heartBeatPacketElement, heartBeatPacketBuilder);
            mHeartBeatPackets[heartBeatServerType] = heartBeatPacketBuilder.getPacket();
        }
    }
}
//-----------------------------------------------------------------------------
roINLINE void RoNetXmlPacketDb::parsePackets(tinyxml2::XMLElement* packets)
{
    if (!packets)
    {
        roLOG_CRIT << "No packet definitions found.";
        return;
    }
    RoXmlElementIterator packetElementIterator(packets, "packet");
    while (packetElementIterator.hasMore())
    {
        tinyxml2::XMLElement* packetElement = packetElementIterator.getNext();
        RoXmlAttributeReader packetAttributes(packetElement);
        RoHashString packetAction = packetAttributes.getHashString("name", "A packet definition should specify a 'name' (action) for the packet.");
        uint16 packetId = packetAttributes.getUInt16("id", "A packet definition should specify a non-zero unsigned 16-bit integer packet 'id'.");
        if (0 == packetId)
        {
            roTHROW(RoXmlPacketDbError()
                << RoErrorInfoDetail("Packet ID should be an unsigned non-zero 16-bit integer.")
                << RoErrorInfoItemName(packetAction.str()));
        }
        uint32 version = packetAttributes.getUInt32("version", "A packet definition should specify a packet-version.");
        if (!hasPacketVersion(version))
        {
            roTHROW(RoXmlPacketDbError()
                << RoErrorInfoDetail("Invalid version number specified for packet.")
                << RoErrorInfoItemName(RoStringUtil::Format("Unknown Version: %d; Packet: %s", version, packetAction.str().c_str())));
        }
        if (roCLIENT_VERSION >= version)
        {
            RoNetPacketBuilder packetBuilder(packetAction);
            packetBuilder.addUInt16(RoDict::PacketId, packetId);
            parseAllData(packetElement, packetBuilder);
            mPackets[packetId] = packetBuilder.getPacket();
            mActionToPacketId[packetAction] = packetId;
        }
    }
}
//-----------------------------------------------------------------------------
roINLINE void RoNetXmlPacketDb::parseAllData(tinyxml2::XMLElement* data, RoNetPacketBuilder& packetBuilder)
{
    RoXmlElementIterator dataIterator(data, "data");
    while (dataIterator.hasMore())
    {
        RoXmlAttributeReader dataAttributes(dataIterator.getNext());
        RoHashString dataType = dataAttributes.getHashString("type", "A data entry should specify the 'type' of the data.");
        if (sXmlDataParsers.count(dataType))
        {
            XmlDataParser parser = sXmlDataParsers.find(dataType)->second;
            parser(*this, dataAttributes, packetBuilder);
        }
        else if (mStructures.count(dataType))
        {
            RoHashString structName = dataAttributes.getHashString("name", roMISSING_DATA_ATTRIBUTE_MSG("struct", "name"));
            packetBuilder.addStructure(structName, mStructures[dataType]);
        }
        else
        {
            roTHROW(RoXmlPacketDbError()
                << RoErrorInfoDetail("Failed to find XML data parser for the specified type.")
                << RoErrorInfoItemName(dataType.str()));
        }
    }
}
