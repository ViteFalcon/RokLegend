////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_SERVER_TYPE_H
#define ROKLEGEND_SERVER_TYPE_H

#include <core/RoPrerequisites.h>

/**
  * Enumeration for different types of server the client connects to.
 **/
enum class RoNetServerType
{
    NONE,

    LOGIN,
    CHARACTER,
    MAP,

    COUNT
};

extern RoString roNetServerTypeToString(RoNetServerType type);

extern RoNetServerType roNetServerTypeFromString(const RoString& serverTypeString);



#ifndef _WIN32

inline size_t hash_value(const RoNetServerType& serverType)
{
    return (size_t)serverType;
}

#endif // _WIN32

#endif // ROKLEGEND_SERVER_TYPE_H
