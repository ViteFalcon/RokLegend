#pragma once
#include "RoTypes.h"
#include <unordered_map>

template <
    class Key,
    class Value,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>
>
using RoHashMap = std::unordered_map < Key, Value, Hash, KeyEqual, RoAllocator<std::pair<const Key, Value>> > ;