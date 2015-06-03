#pragma once
#include "RoAllocator.h"
#include <unordered_set>

template <
    class Key,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>
>
using RoHashSet = std::unordered_set < Key, Hash, KeyEqual, RoAllocator<Key> > ;
