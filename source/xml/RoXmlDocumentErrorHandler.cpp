////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoXmlDocumentErrorHandler.h"
#include "RoXmlExceptions.h"
#include "tinyxml/tinyxml2.h"

#include <core/RoErrorInfo.h>

roERROR_INFO(TinyXMLErrorString1, RoString);
roERROR_INFO(TinyXMLErrorString2, RoString);

RoXmlDocumentErrorHandler::RoXmlDocumentErrorHandler(tinyxml2::XMLDocument& document)
    : mDocument(document)
{
}

RoXmlDocumentErrorHandler::~RoXmlDocumentErrorHandler()
{
}

const RoXmlDocumentErrorHandler& RoXmlDocumentErrorHandler::operator = (int errorCode) const
{
    if (tinyxml2::XML_NO_ERROR != errorCode)
    {
        roTHROW(RoTinyXmlError()
            << RoErrorInfoDetail("Failed to load packet DB.")
            << RoErrorInfoTinyXMLErrorString1(mDocument.GetErrorStr1())
            << RoErrorInfoTinyXMLErrorString2(mDocument.GetErrorStr2()));
    }
    return *this;
}
