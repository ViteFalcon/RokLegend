////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_XML_ATTRIBUTE_READER_H
#define ROKLEGEND_XML_ATTRIBUTE_READER_H

#include <core/RoPrerequisites.h>

namespace tinyxml2
{
    class XMLElement;
}

/**
  * Attribute reader for tinyxml2.
 **/
class RoXmlAttributeReader
{
public:
    RoXmlAttributeReader(tinyxml2::XMLElement* element);

    bool has(const char* attributeName) const;

    bool getBool(const char* attributeName, const char* errorHint) const;
    roINLINE bool tryToGetBool(const char* attributeName, bool defaultValue) const
    {
        bool val = defaultValue;
        try
        {
            val = getBool(attributeName, "");
        }
        catch(...)
        {
        }
        return val;
    }

    uint8 getByte(const char* attributeName, const char* errorHint) const;
    roINLINE uint8 tryToGetByte(const char* attributeName, uint8 defaultValue) const
    {
        uint8 val = defaultValue;
        try
        {
            val = getByte(attributeName, "");
        }
        catch(...)
        {
        }
        return val;
    }

    uint16 getUInt16(const char* attributeName, const char* errorHint) const;
    roINLINE uint16 tryToGetUInt16(const char* attributeName, uint16 defaultValue) const
    {
        uint16 val = defaultValue;
        try
        {
            val = getUInt16(attributeName, "");
        }
        catch(...)
        {
        }
        return val;
    }

    uint32 getUInt32(const char* attributeName, const char* errorHint) const;
    roINLINE uint32 tryToGetUInt32(const char* attributeName, uint32 defaultValue) const
    {
        uint32 val = defaultValue;
        try
        {
            val = getUInt32(attributeName, "");
        }
        catch(...)
        {
        }
        return val;
    }

    RoString getString(const char* attributeName, const char* errorHint) const;
    roINLINE RoString tryToGetString(const char* attributeName, const RoString& defaultValue) const
    {
        RoString val = defaultValue;
        try
        {
            val = getString(attributeName, "");
        }
        catch(...)
        {
        }
        return val;
    }

    RoHashString getHashString(const char* attributeName, const char* errorHint) const;
    RoHashString tryToGetHashString(const char* attributeName, const RoHashString& defaultValue) const
    {
        RoHashString val = defaultValue;
        try
        {
            val = getHashString(attributeName, "");
        }
        catch(...)
        {
        }
        return val;
    }
private:
    tinyxml2::XMLElement* mElement;
};

#endif // ROKLEGEND_XML_ATTRIBUTE_READER_H
