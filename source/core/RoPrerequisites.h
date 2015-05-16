#pragma once
#ifndef ROKLEGEND_PREREQUISITES_H
#define ROKLEGEND_PREREQUISITES_H

#include "RoTypes.h"
#include "RoString.h"
#include "RoHashString.h"

#include <functional>

#include <boost/signals2.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

typedef RoHashString RoActionId;

class RoOgreRenderer;

typedef std::vector<std::string>        StringArray;
typedef std::vector<RoString>           RoStringArray;
typedef boost::unordered_set<RoString>  RoStringSet;

typedef optional<RoString>   RoOptionalUtf8;
typedef optional<RoString>   RoOptionalString;

#include "RoSharedPtr.h"

typedef uint64 RoEntityId;

roFORWARD_DECL_PTR(RoEntity);

typedef std::vector<RoEntityPtr> RoEntityArray;

enum RoEventResult
{
    ///< Propagates the event to the next in line for event
    eEvent_Propogate = 0x001,
    ///< Indicates the event was not handled
    eEvent_Unhandled = 0x010,
    ///< Indicates the event was handled
    eEvent_Handled = 0x020,
    ///< Indicates that the event was not handled and should be propogated
    eEvent_PropogateUnhandled = eEvent_Propogate|eEvent_Unhandled,
    ///< Indicates that the event was handled and should be propogated
    eEvent_PropogateHandled = eEvent_Propogate|eEvent_Handled
};

#endif // ROKLEGEND_PREREQUISITES_H
