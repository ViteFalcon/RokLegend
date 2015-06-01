////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoXmlAttributeReader.h"
#include "RoXmlExceptions.h"

#include "tinyxml/tinyxml2.h"

#include <core/RoStringUtil.h>

#define roTHROW_XML_ATTRIBUTE_ERROR(dataType, attributeName, errorHint) roTHROW(RoTinyXmlError() \
    << RoErrorInfoDetail("Failed to retrieve attribute value as " dataType ".") \
    << RoErrorInfoItemName(attributeName) \
    << RoErrorInfoHint(errorHint))

RoXmlAttributeReader::RoXmlAttributeReader(tinyxml2::XMLElement* element)
    : mElement(element)
{
}

bool RoXmlAttributeReader::has(const char* attributeName) const
{
    return NULL != mElement->Attribute(attributeName);
}

bool RoXmlAttributeReader::getBool(const char* attributeName, const char* errorHint) const
{
    const char* val = mElement->Attribute(attributeName);
    if (!val)
    {
        roTHROW_XML_ATTRIBUTE_ERROR("bool", attributeName, errorHint);
    }
    RoOptionalBool boolValue = RoStringUtil::ParseBool(val);
    if (!boolValue.is_initialized())
    {
        roTHROW_XML_ATTRIBUTE_ERROR("bool", attributeName, errorHint);
    }
    return boolValue.get();
}

uint8 RoXmlAttributeReader::getByte(const char* attributeName, const char* errorHint) const
{
    uint32 val;
    if (mElement->QueryUnsignedAttribute(attributeName, &val) != tinyxml2::XML_NO_ERROR)
    {
        roTHROW_XML_ATTRIBUTE_ERROR("byte", attributeName, errorHint);
    }
    return (uint8)val;
}

uint16 RoXmlAttributeReader::getUInt16(const char* attributeName, const char* errorHint) const
{
    uint32 val;
    if (mElement->QueryUnsignedAttribute(attributeName, &val) != tinyxml2::XML_NO_ERROR)
    {
        roTHROW_XML_ATTRIBUTE_ERROR("unsigned-integer (16-bit)", attributeName, errorHint);
    }
    return (uint16)val;
}

uint32 RoXmlAttributeReader::getUInt32(const char* attributeName, const char* errorHint) const
{
    uint32 val;
    if (mElement->QueryUnsignedAttribute(attributeName, &val) != tinyxml2::XML_NO_ERROR)
    {
        roTHROW_XML_ATTRIBUTE_ERROR("unsigned-integer (32-bit)", attributeName, errorHint);
    }
    return val;
}

RoString RoXmlAttributeReader::getString(const char* attributeName, const char* errorHint) const
{
    const char* val = mElement->Attribute(attributeName);
    if (!val)
    {
        roTHROW_XML_ATTRIBUTE_ERROR("string", attributeName, errorHint);
    }
    return RoString(val);
}

RoHashString RoXmlAttributeReader::getHashString(const char* attributeName, const char* errorHint) const
{
    const char* val = mElement->Attribute(attributeName);
    if (!val)
    {
        roTHROW_XML_ATTRIBUTE_ERROR("hash-string", attributeName, errorHint);
    }
    return RoHashString(val);
}
