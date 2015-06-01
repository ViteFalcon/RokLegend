////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_XML_DOCUMENT_ERROR_HANDLER_H
#define ROKLEGEND_XML_DOCUMENT_ERROR_HANDLER_H

#include <core/RoPrerequisites.h>

namespace tinyxml2
{
    class XMLDocument;
}

/**
  * Error handler for TinyXML Document.
 **/
class RoXmlDocumentErrorHandler
{
public:
    RoXmlDocumentErrorHandler(tinyxml2::XMLDocument& document);
    ~RoXmlDocumentErrorHandler();

    const RoXmlDocumentErrorHandler& operator = (int errorCode) const;

private:
    tinyxml2::XMLDocument& mDocument;
};

#endif // ROKLEGEND_XML_DOCUMENT_ERROR_HANDLER_H
