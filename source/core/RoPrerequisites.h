#pragma once
#ifndef ROKLEGEND_PREREQUISITES_H
#define ROKLEGEND_PREREQUISITES_H

#include "RoTypes.h"
#include "RoString.h"
#include "RoHashString.h"

#include <algorithm>
#include <functional>

#include <boost/signals2.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

using RoActionId = RoHashString;

class RoOgreRenderer;

using RoAtomicString = RoAtomic < RoString > ;
using StringArray = RoVector<std::string>;
using RoStringArray = RoVector<RoString>;
using RoStringSet = boost::unordered_set<RoString>;

using RoOptionalUtf8 = optional < RoString > ;
using RoOptionalString = optional < RoString > ;

#define roMAX(a,b) std::max(a,b)
#define roMIN(a,b) std::min(a,b)

#include "RoSharedPtr.h"

using RoTaskId = uint64;

enum class RoEventResult
{
    ///< Propagates the event to the next in line for event
    PROPAGATE = 0x001,
    ///< Indicates the event was not handled
    UNHANDLED = 0x010,
    ///< Indicates the event was handled
    HANDLED = 0x020,
    ///< Indicates that the event was not handled and should be propagated
    PROPAGATE_UNHANDLED = PROPAGATE|UNHANDLED,
    ///< Indicates that the event was handled and should be propagated
    PROPAGATE_HANDLED = PROPAGATE|HANDLED
};

#define roPOST_MSG(actionName, args)

#endif // ROKLEGEND_PREREQUISITES_H
