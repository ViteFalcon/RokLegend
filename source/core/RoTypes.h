#pragma once
#ifndef RO_TYPES_H
#define RO_TYPES_H

#include "RoDefines.h"
#include <cstdint>
#include <boost/optional.hpp>
#include <string>
#include <memory>

// TODO: Move this to CMake
#define roUSE_TBB_ALLOCATORS 1
#if roUSE_TBB_ALLOCATORS
#   include <tbb/scalable_allocator.h>

template <typename T>
using RoAllocator = tbb::scalable_allocator < T > ;

#else

template <typename T>
using RoAllocator = std::allocator < T > ;
#endif

#include "RoVector.h"

using int8 = std::int8_t;
using uint8 = std::uint8_t;

using int16 = std::int16_t;
using uint16 = std::uint16_t;

using int32 = std::int32_t;
using uint32 = std::uint32_t;

using int64 = std::int64_t;
using uint64 = std::uint64_t;

using float32 = float;
// DO NOT uncomment this. Eigen will have compilation issues if we define a 'real' type.
// https://forum.kde.org/viewtopic.php?f=74&t=117258
// using real = float32;
using float64 = double;

using RoBuffer = std::string;
using RoStreamSize = size_t;

using RoCharArray = RoVector<char>;
using RoByteArray = RoVector<uint8>;

template <typename T>
using optional = boost::optional<T>;

template <typename T>
using shared_ptr = std::shared_ptr<T>;

template <typename T>
using unique_ptr = std::unique_ptr<T>;

using RoOptionalBool = optional<bool>;
using RoOptionalInt8 = optional<int8>;
using RoOptionalUInt8 = optional<uint8>;
using RoOptionalInt16 = optional<int16>;
using RoOptionalUInt16 = optional<uint16>;
using RoOptionalInt32 = optional<int32>;
using RoOptionalUInt32 = optional<uint32>;
using RoOptionalInt64 = optional<int64>;
using RoOptionalUInt64 = optional<uint64>;
using RoOptionalFloat = optional<float>;
using RoOptionalDouble = optional<double>;

#endif // RO_TYPES_H
