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

using RoActionId = RoHashString;

class RoOgreRenderer;

using StringArray = RoVector<std::string> ;
using RoStringArray = RoVector<RoString>;
typedef boost::unordered_set<RoString>  RoStringSet;

typedef optional<RoString>   RoOptionalUtf8;
typedef optional<RoString>   RoOptionalString;

#include "RoSharedPtr.h"

using RoTaskId = uint64;

enum class RoEventResult
{
    ///< Propagates the event to the next in line for event
    Propogate = 0x001,
    ///< Indicates the event was not handled
    Unhandled = 0x010,
    ///< Indicates the event was handled
    Handled = 0x020,
    ///< Indicates that the event was not handled and should be propagated
    PropogateUnhandled = Propogate|Unhandled,
    ///< Indicates that the event was handled and should be propagated
    PropogateHandled = Propogate|Handled
};

#endif // ROKLEGEND_PREREQUISITES_H
