////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoXmlElementIterator.h"
#include "tinyxml/tinyxml2.h"

RoXmlElementIterator::RoXmlElementIterator(tinyxml2::XMLElement* parentElement, const char* childElementName)
    : mElementName(childElementName)
    , mElement(parentElement->FirstChildElement(childElementName))
{
}

tinyxml2::XMLElement* RoXmlElementIterator::getNext()
{
    tinyxml2::XMLElement* nextElement = mElement;
    if (mElement)
    {
        mElement = mElement->NextSiblingElement(mElementName);
    }
    return nextElement;
}
