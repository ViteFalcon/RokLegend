////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_XML_ELEMENT_ITERATOR_H
#define ROKLEGEND_XML_ELEMENT_ITERATOR_H

#include <core/RoPrerequisites.h>

namespace tinyxml2
{
    class XMLElement;
}

/**
  * Element iterator for TinyXML.
 **/
class RoXmlElementIterator
{
public:
    RoXmlElementIterator(tinyxml2::XMLElement* parentElement, const char* childElementName);

    roINLINE bool hasMore() const
    {
        return 0 != mElement;
    }

    tinyxml2::XMLElement* getNext();

private:
    const char* mElementName;
    tinyxml2::XMLElement* mElement;
};

#endif // ROKLEGEND_XML_ELEMENT_ITERATOR_H
