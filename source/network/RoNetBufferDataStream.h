////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <core/RoPrerequisites.h>
#include <core/RoDataStream.h>

#include <sstream>

/**
  * Buffer data stream for network module
 **/
class RoNetBufferDataStream : public RoDataStream
{
public:
    RoNetBufferDataStream() = default;
    virtual ~RoNetBufferDataStream() = default;

    virtual RoDataStream::AccessMode getAccessMode() const override
    {
        return RoDataStream::AccessMode::READ_WRITE;
    }

    virtual RoStreamSize write(const void* buffer, RoStreamSize size)
    {
        mStream.write((char*)buffer, size);
        return size;
    }

    virtual RoStreamSize read(void* buffer, RoStreamSize size)
    {
        mStream.read((char*)buffer, size);
        return size;
    }

    virtual RoBuffer readAll() override;

    virtual bool empty() const;

    virtual RoStreamSize size() const;

    virtual RoStreamSize remainingSize(void) const;

    virtual void seek(RoStreamSize pos)
    {
        mStream.seekp(pos);
        mStream.seekg(pos);
    }

    virtual RoStreamSize tell()
    {
        return mStream.tellp();
    }

    virtual void advance(RoStreamSize bytes)
    {
        mStream.seekg(bytes);
    }

    virtual RoStreamSize readPosition()
    {
        return mStream.tellg();
    }

    virtual bool eof() const
    {
        return mStream.eof();
    }

    inline virtual RoStreamSize bytesRead(void) const
    {
        return 0;
    }

    virtual void close(void)
    {
    }

    RoBuffer data() const
    {
        const std::string charBuffer = mStream.str();
        RoBuffer buffer{};
        buffer.assign(charBuffer.begin(), charBuffer.end());
        return buffer;
    }

private:

    std::stringstream mStream;
};
