////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_TYPES_H
#define ROKLEGEND_NET_TYPES_H

#include <core/RoPrerequisites.h>
#include <core/RoIndexedMap.h>

#include "RoNetData.h"
#include "RoNetString.h"

typedef RoNetDataT<bool>    RoNetBool;
typedef RoNetDataT<int8>    RoNetInt8;
typedef RoNetDataT<uint8>   RoNetUInt8;
typedef RoNetDataT<int16>   RoNetInt16;
typedef RoNetDataT<uint16>  RoNetUInt16;
typedef RoNetDataT<int32>   RoNetInt32;
typedef RoNetDataT<uint32>  RoNetUInt32;
typedef RoNetDataT<int64>   RoNetInt64;
typedef RoNetDataT<uint64>  RoNetUInt64;

typedef RoIndexedMap<RoHashString, RoNetDataPtr> RoHashDataMap;

typedef RoVector<RoNetDataPtr> RoNetDataPtrArray;

#endif // ROKLEGEND_NET_TYPES_H
