#pragma once
#ifndef RO_TYPES_H
#define RO_TYPES_H

#include "RoDefines.h"
#include <boost/cstdint.hpp>
#include <boost/optional.hpp>
#include <string>
#include <vector>
#include <memory>

using int8 = boost::int8_t;
using uint8 = boost::uint8_t;

using int16 = boost::int16_t;
using uint16 = boost::uint16_t;

using int32 = boost::int32_t;
using uint32 = boost::uint32_t;

using int64 = boost::int64_t;
using uint64 = boost::uint64_t;

using float32 = float;
using real = float32;
using float64 = double;

using RoBuffer = std::string;
using RoStreamSize = boost::uintmax_t;

using RoCharArray = std::vector<char>;
using RoByteArray = std::vector<uint8>;

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
