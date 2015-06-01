////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoNetData.h"

#include <core/RoException.h>
#include <core/RoErrorInfo.h>

roDEFINE_EXCEPTION(NetworkDataException);

RoDataStream& operator << (RoDataStream& stream, const RoNetDataPtr& data)
{
    data->writeToStream(stream);
    return stream;
}

RoDataStream& operator >> (RoDataStream& stream, RoNetDataPtr& data)
{
    data->readFromStream(stream);
    return stream;
}

namespace ro
{
    namespace detail
    {
        void roWriteToDataStream(RoDataStream& stream, const RoNetDataPtr& data)
        {
            data->writeToStream(stream);
        }

        void roReadFromDataStream(RoDataStream& stream, RoNetDataPtr& data)
        {
            data->readFromStream(stream);
        }
    }
}

RoNetData::RoNetData(const RoHashString& name)
    : mName(name)
{
}

RoNetData::~RoNetData()
{
}

void RoNetData::readFromStream(RoDataStream& stream)
{
    if (!stream.isReadable())
    {
        roTHROW(RoNetworkDataException()
            << RoErrorInfoDetail("Cannot read from a write-only stream.")
        );
    }
    readImpl(stream);
}

void RoNetData::writeToStream(RoDataStream& stream) const
{
    if (!stream.isWritable())
    {
        roTHROW(RoNetworkDataException()
            << RoErrorInfoDetail("Cannot write to a read-only stream.")
        );
    }
    mWriteOffset = stream.tell();
    writeImpl(stream);
}

void RoNetData::RewriteDataToStream(RoNetDataPtr data, RoDataStream& stream)
{
    if (!data->mWriteOffset.is_initialized())
    {
        roTHROW(RoInvalidOperation()
            << RoErrorInfoDetail("Cannot re-write data to stream.")
            << RoErrorInfoHint("Has this data been previously written to a stream to re-write it?")
            << RoErrorInfoItemName(data->getName()));
    }
    RoStreamSize writeOffset = data->mWriteOffset.get();
    RoStreamSize streamStartPosition = stream.tell();
    stream.seek(data->mWriteOffset.get());
    data->writeToStream(stream);
    stream.seek(streamStartPosition);
}
