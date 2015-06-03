#pragma once
#include "RoAllocator.h"
#include <vector>

template <typename T>
using RoVector = std::vector < T, RoAllocator<T> > ;
