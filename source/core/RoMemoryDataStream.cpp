#include "RoMemoryDataStream.h"
#include "RoException.h"
#include "RoErrorInfo.h"
#include "RoUtilities.h"

#include <cassert>

#define roSTRSTREAM_MODE 3

//=============================================================================
namespace {

RoStreamSize BufferFromInputStream(
    char** outBuffer,
    std::istream& inStream)
{
    std::streampos currentPos = inStream.tellg();
    inStream.seekg(std::ios::end);
    std::streampos size = inStream.tellg() - currentPos;
    inStream.seekg(currentPos);
    *outBuffer = new char[size_t(size)];
    inStream.read(*outBuffer, size);

    //// Read the last character to force eof of stream
    //char ch;
    //assert(0 == inStream.get(ch));
    //(void)ch;

    return size;
}

}
//=============================================================================
RoMemoryDataStream::RoMemoryDataStream(size_t size)
    : mAccessMode(AccessMode::READ_WRITE)
    , mBuffer(new char[size])
    , mCursor(mBuffer)
    , mEnd(mBuffer + size)
    , mFreeOnClose(true)
{
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(const RoString& name, size_t size)
    : mAccessMode(AccessMode::READ_WRITE)
    , mBuffer(new char[size])
    , mCursor(mBuffer)
    , mEnd(mBuffer + size)
    , mFreeOnClose(true)
{
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(char* bufferPointer, size_t size, bool freeOnClose, AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(bufferPointer)
    , mCursor(mBuffer)
    , mEnd(mBuffer + size)
    , mFreeOnClose(freeOnClose)
{
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(const RoString& name, char* bufferPointer, size_t size, bool freeOnClose, AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(bufferPointer)
    , mCursor(mBuffer)
    , mEnd(mBuffer + size)
    , mFreeOnClose(freeOnClose)
{
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(std::istream& stream, AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(0)
    , mCursor(0)
    , mEnd(0)
    , mFreeOnClose(true)
{
    RoStreamSize size = BufferFromInputStream(&mBuffer, stream);
    mCursor = mBuffer;
    mEnd = mBuffer + size;
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(
    const RoString& name,
    std::istream& stream,
    AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(0)
    , mCursor(0)
    , mEnd(0)
    , mFreeOnClose(true)
{
    RoStreamSize size = BufferFromInputStream(&mBuffer, stream);
    mCursor = mBuffer;
    mEnd = mBuffer + size;
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(
    std::istream& stream,
    RoStreamSize size,
    AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(new char[size_t(size)])
    , mCursor(mBuffer)
    , mEnd(mBuffer + size)
    , mFreeOnClose(true)
{
    stream.read(mBuffer, std::streamsize(size));
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(
    const RoString& name,
    std::istream& stream,
    RoStreamSize size,
    AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(new char[size_t(size)])
    , mCursor(mBuffer)
    , mEnd(mBuffer + size)
    , mFreeOnClose(true)
{
    stream.read(mBuffer, std::streamsize(size));
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(RoDataStream& sourceStream, AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(0)
    , mCursor(0)
    , mEnd(0)
    , mFreeOnClose(true)
{
    RoBuffer buffer = sourceStream.readAll();
    mBuffer = new char[buffer.size()];
    mCursor = mBuffer;
    mEnd = mBuffer + buffer.size();
    memcpy(mBuffer, &buffer[0], buffer.size());
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(const RoString& name, RoDataStream& sourceStream, AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(0)
    , mCursor(0)
    , mEnd(0)
    , mFreeOnClose(true)
{
    RoBuffer buffer = sourceStream.readAll();
    mBuffer = new char[buffer.size()];
    mCursor = mBuffer;
    mEnd = mBuffer + buffer.size();
    memcpy(mBuffer, &buffer[0], buffer.size());
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(RoDataStreamPtr sourceStream, AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(0)
    , mCursor(0)
    , mEnd(0)
    , mFreeOnClose(true)
{
    RoBuffer buffer = sourceStream->readAll();
    mBuffer = new char[buffer.size()];
    mCursor = mBuffer;
    mEnd = mBuffer + buffer.size();
    memcpy(mBuffer, &buffer[0], buffer.size());
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::RoMemoryDataStream(const RoString& name, RoDataStreamPtr sourceStream, AccessMode accessMode)
    : mAccessMode(accessMode)
    , mBuffer(0)
    , mCursor(0)
    , mEnd(0)
    , mFreeOnClose(true)
{
    RoBuffer buffer = sourceStream->readAll();
    mBuffer = new char[buffer.size()];
    mCursor = mBuffer;
    mEnd = mBuffer + buffer.size();
    memcpy(mBuffer, &buffer[0], buffer.size());
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::~RoMemoryDataStream()
{
    if (mFreeOnClose)
    {
        delete[] mBuffer;
    }
}
//-----------------------------------------------------------------------------
RoMemoryDataStream::AccessMode RoMemoryDataStream::getAccessMode() const
{
    return mAccessMode;
}
//-----------------------------------------------------------------------------
//RoStreamSize RoMemoryDataStream::read(void* buffer, RoStreamSize size)
//{
//    assert(mEnd > mCursor + size);
//    memcpy(buffer, mCursor, size_t(size));
//    mCursor += size;
//    return size;
//}
//-----------------------------------------------------------------------------
RoStreamSize RoMemoryDataStream::write(const void* buffer, RoStreamSize size)
{
    if (!isWritable())
    {
        roTHROW(RoInvalidOperation()
            << RoErrorInfoDetail("Cannot write to a read-only data-stream!"));
    }
    assert(mEnd >= mCursor + size);
    RoUtilities::MemCopy(mCursor, mEnd - mCursor, buffer, size_t(size));
    mCursor += size;
    return size;
}
//------------------------------------------------------------------------------
RoBuffer RoMemoryDataStream::readAll()
{
    RoBuffer buffer(mBuffer, mEnd-mCursor);
    return buffer;
}