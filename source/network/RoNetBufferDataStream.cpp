////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoNetBufferDataStream.h"

RoBuffer RoNetBufferDataStream::readAll()
{
    RoBuffer buffer{};
    buffer.resize(remainingSize(), 0);
    mStream.read(reinterpret_cast<char*>(&buffer[0]), buffer.size());
    return buffer;
}

bool RoNetBufferDataStream::empty() const
{
    return 0 == remainingSize();
}

RoStreamSize RoNetBufferDataStream::size() const
{
    // ARGH! HACK!
    std::stringstream& stream = const_cast<std::stringstream&>(mStream);
    return stream.tellp();
}

RoStreamSize RoNetBufferDataStream::remainingSize(void) const
{
    std::stringstream& stream = const_cast<std::stringstream&>(mStream);
    RoStreamSize cur_pos = stream.tellg();
    stream.seekg(std::ios::end);
    RoStreamSize end_pos = stream.tellg();
    stream.seekg(cur_pos);
    return end_pos - cur_pos;
}
