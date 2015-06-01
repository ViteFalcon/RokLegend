////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_SOCKET_INPUT_STREAM_H
#define ROKLEGEND_NET_SOCKET_INPUT_STREAM_H

#include <core/RoPrerequisites.h>
#include <core/RoDataStream.h>
#include <core/RoException.h>
#include <core/RoLog.h>

#include <boost/asio.hpp>

/**
  * Data input-stream for network sockets.
 **/
template <class SocketType, class BufferType>
class RoNetSocketInputStream : public RoDataStream
{
public:
    typedef SocketType      _MySocketType;
    typedef BufferType      _MyBufferType;
    typedef _MySocketType&  _MySocketRef;

    RoNetSocketInputStream(_MySocketRef socket)
        : mSocket(socket)
        , mSize(0)
        , mEof(false)
    {
    }

    virtual ~RoNetSocketInputStream()
    {
    }

    virtual RoDataStream::AccessMode getAccessMode() const override
    {
        return AccessMode::READ;
    }

    virtual RoStreamSize read(void* data, RoStreamSize size)
    {
        if (mEof)
        {
            roTHROW(RoInvalidOperation()
                << RoErrorInfoDetail("Cannot read from a socket stream that's reached it's end."));
        }
        const RoStreamSize bytesRead = (RoStreamSize)boost::asio::read(
            mSocket,
            boost::asio::buffer(data, size_t(size)),
            boost::asio::transfer_exactly(size_t(size)));

        if (bytesRead != size)
        {
            mEof = true;
        }
        mSize += bytesRead;
        return bytesRead;
    }

    virtual RoBuffer readAll() override
    {
        return RoBuffer{};
    }

    virtual void seek(RoStreamSize pos)
    {
        roLOG_WARN << "Cannot move read/write cursor within a network data stream. Are you using the correct stream for the purpose?";
    }
    virtual RoStreamSize tell()
    {
        return mSize;
    }
    virtual bool eof() const
    {
        return mEof;
    }

    virtual RoStreamSize size() const
    {
        return mSize;
    }

    virtual bool empty() const
    {
        return mEof;
    }

    virtual RoStreamSize remainingSize(void) const
    {
        return 0;
    }

    virtual void close(void)
    {
        // Do nothing
    }

    virtual void advance(RoStreamSize bytesToAdvance)
    {
        mSize += bytesToAdvance;
    }

private:
    _MySocketRef    mSocket;
    _MyBufferType   mBuffer;
    RoStreamSize    mSize;
    bool            mEof;
};

#endif // ROKLEGEND_NET_SOCKET_INPUT_STREAM_H
