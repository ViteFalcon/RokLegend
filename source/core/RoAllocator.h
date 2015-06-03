#pragma once
// TODO: Move this to CMake
#define roUSE_TBB_ALLOCATORS 1
#if roUSE_TBB_ALLOCATORS
#   include <tbb/scalable_allocator.h>

template <typename T>
using RoAllocator = tbb::scalable_allocator < T > ;

#else
#   include <allocators>

template <typename T>
using RoAllocator = std::allocator < T > ;
#endif