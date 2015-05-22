#pragma once
#ifndef ROKLEGEND_MEMORY_DATA_STREAM_H
#define ROKLEGEND_MEMORY_DATA_STREAM_H

#include "RoDataStream.h"

#include <sstream>

class RoMemoryDataStream : public RoDataStream
{
public:
    /**
     * Create an in-memory data-stream of pre-defined size
     **/
    explicit RoMemoryDataStream(size_t size);
    /**
     * Create a named in-memory data-stream of pre-defined size.
     **/
    RoMemoryDataStream(const RoString& name, size_t size);

    /**
     * Create an in-memory data-stream from an existing buffer
     **/
    RoMemoryDataStream(char* buffer, size_t size, bool freeOnClose = false, AccessMode accessMode = AccessMode::READ_WRITE);
    /**
     * Create a named in-memory data-stream from an existing buffer
     **/
    RoMemoryDataStream(const RoString& name, char* buffer, size_t size, bool freeOnClose = false, AccessMode accessMode = AccessMode::READ_WRITE);

    /**
     * Create an in-memory data-stream from an std::istream instance
     **/
    explicit RoMemoryDataStream(std::istream& stream, AccessMode accessMode = AccessMode::READ_WRITE);
    /**
     * Create a named, in-memory data-stream from an std::istream instance
     **/
    RoMemoryDataStream(const RoString& name, std::istream& stream, AccessMode accessMode = AccessMode::READ_WRITE);

    /**
     * Create an in-memory data-stream from an std::istream instance
     **/
    RoMemoryDataStream(std::istream& stream, RoStreamSize size, AccessMode accessMode = AccessMode::READ_WRITE);
    /**
     * Create a named, in-memory data-stream from an std::istream instance
     **/
    RoMemoryDataStream(const RoString& name, std::istream& stream, RoStreamSize size, AccessMode accessMode = AccessMode::READ_WRITE);

    /**
     * Create an in-memory data-stream from a different type of data-stream instance
     **/
    explicit RoMemoryDataStream(RoDataStream& sourceStream, AccessMode accessMode = AccessMode::READ_WRITE);
    /**
     * Create a named in-memory data-stream from a different type of data-stream instance
     **/
    RoMemoryDataStream(const RoString& name, RoDataStream& sourceStream, AccessMode accessMode = AccessMode::READ_WRITE);

    /**
     * Create an in-memory data-stream from a different type of data-stream instance
     **/
    explicit RoMemoryDataStream(RoDataStreamPtr sourceStream, AccessMode accessMode = AccessMode::READ_WRITE);
    /**
     * Create a named in-memory data-stream from a different type of data-stream instance
     **/
    RoMemoryDataStream(const RoString& name, RoDataStreamPtr sourceStream, AccessMode accessMode = AccessMode::READ_WRITE);

    ~RoMemoryDataStream(void);

    virtual AccessMode getAccessMode() const override;

    inline virtual RoStreamSize size(void) const override
    {
        return mEnd - mBuffer;
    }

    inline virtual RoStreamSize remainingSize(void) const override
    {
        return mEnd - mCursor;
    }

    virtual bool empty() const override
    {
        return mEnd >= mCursor;
    }

    inline virtual RoStreamSize read(void* buffer, RoStreamSize size) override
    {
        assert(mEnd >= mCursor + size);
        memcpy(buffer, mCursor, size_t(size));
        mCursor += size;
        return size;
    }

    virtual RoBuffer readAll() override;

    inline virtual std::wstring readUtf16()
    {
        std::wstring str(reinterpret_cast<std::wstring::value_type*>(mCursor));
        mCursor += (str.length() + 1) * sizeof(RoString::value_type);
        return str;
    }

    inline virtual std::string readUtf8()
    {
        std::string str(mCursor);
        mCursor += str.length() + 1;
        return str;
    }

    virtual RoStreamSize write(const void* buffer, RoStreamSize size);

    inline virtual void seek(RoStreamSize pos)
    {
        assert(mEnd >= mCursor + pos);
        mCursor = mBuffer + pos;
    }

    virtual void advance(RoStreamSize bytes)
    {
        mCursor += bytes;
    }

    inline virtual RoStreamSize tell()
    {
        return mCursor - mBuffer;
    }

    inline virtual bool eof() const
    {
        return mEnd <= mCursor;
    }

    inline virtual void close(void)
    {
        // Do nothing
    }

private:
    const AccessMode mAccessMode;
    char*   mBuffer;
    char*   mCursor;
    char*   mEnd;
    bool    mFreeOnClose;
};

#endif // ROKLEGEND_MEMORY_DATA_STREAM_H
