SET(TINYXML_SOURCES
	tinyxml/tinyxml2.cpp
	tinyxml/tinyxml2.h
)
RO_SOURCE_GROUP(tinyxml ${TINYXML_SOURCES})

SET(RO_XML_SOURCES
	${TINYXML_SOURCES}
	CMakeLists.txt
	RoXmlAttributeReader.cpp
	RoXmlAttributeReader.h
	RoXmlDocumentErrorHandler.cpp
	RoXmlDocumentErrorHandler.h
	RoXmlElementIterator.cpp
	RoXmlElementIterator.h
	RoXmlExceptions.h
)

ADD_LIBRARY(${RO_XML_LIBRARY} STATIC ${RO_XML_SOURCES})