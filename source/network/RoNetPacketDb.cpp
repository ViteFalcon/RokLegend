////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoNetPacketDb.h"
#include "RoNetXmlPacketDb.h"

#include <core/RoException.h>
#include <core/RoErrorInfo.h>

roDEFINE_EXCEPTION(InvalidPacketDbSource);

const RoHashString RoNetPacketDb::TypeByte = "byte";
const RoHashString RoNetPacketDb::TypeBool = "bool";
const RoHashString RoNetPacketDb::TypeUInt16 = "uint16";
const RoHashString RoNetPacketDb::TypeUInt32 = "uint32";
const RoHashString RoNetPacketDb::TypeString = "string";
const RoHashString RoNetPacketDb::TypeList = "list";
const RoHashString RoNetPacketDb::ConditionBytesRead = "bytes_read";
const RoHashString RoNetPacketDb::ConditionRestOfPacket = "rest_of_packet";

RoNetPacketDbPtr RoNetPacketDb::LoadFromSource(const RoString& source)
{
    if (RoNetXmlPacketDb::CanReadDbSource(source))
        return RoNetPacketDbPtr(new RoNetXmlPacketDb(source));
    roTHROW(RoInvalidPacketDbSource()
        << RoErrorInfoDetail("Invalid packet DB source.")
        << RoErrorInfoItemName(source));
}

RoNetPacketDb::RoNetPacketDb(const RoString& source)
    : mSource(source)
{
}

RoNetPacketDb::~RoNetPacketDb()
{
}
