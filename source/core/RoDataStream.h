#pragma once
#include "RoPrerequisites.h"
#include "RoSharedPtr.h"

class RoDataStream
{
public:
    enum class AccessMode
    {
        READ = 0x1,
        WRITE = 0x2,
        READ_WRITE = READ|WRITE
    };

public:
    RoDataStream() = default;
    virtual ~RoDataStream() = default;

    virtual AccessMode getAccessMode() const = 0;

    bool isReadable() const
    {
        return 0 != ((int)AccessMode::READ & (int)getAccessMode());
    }

    bool isWritable() const
    {
        return 0 != ((int)AccessMode::WRITE & (int)getAccessMode());
    }

    virtual RoStreamSize read(void* buffer, RoStreamSize size) = 0;

    template <typename T>
    RoDataStream& read(T& val)
    {
        read(&val, sizeof(val));
        return *this;
    }

    virtual RoBuffer readAll() = 0;

    template <typename _RoStringType>
    _RoStringType readString(const typename _RoStringType::value_type delimiter)
    {
        typedef typename _RoStringType::value_type _RoCharType;
        std::list<_RoCharType> charList;
        _RoCharType character;
        do
        {
            read(&character, sizeof(_RoCharType));
            charList.push_back(character);
        } while (delimiter != character);

        if (0 != (int)delimiter)
        {
            charList.push_back((_RoCharType)0);
        }

        _RoStringType result;
        result.assign(charList.begin(), charList.end());
        return result;
    }

    virtual std::wstring readUtf16()
    {
        std::wstring val = readString<std::wstring>(0);
        return RoString(val);
    }

    virtual std::string readUtf8()
    {
        std::string val = readString<std::string>(0);
        return RoString(val);
    }

    virtual RoStreamSize write(const void* buffer, RoStreamSize size)
    {
        (void)buffer;
        (void)size;
        // Default to not supported.
        return 0;
    }

    virtual RoStreamSize bytesRead(void) const
    {
        return size() - remainingSize();
    }

    virtual void seek(RoStreamSize pos) = 0;
    virtual RoStreamSize tell() = 0;
    virtual bool eof() const = 0;
    virtual void advance(RoStreamSize bytes) = 0;

    virtual RoStreamSize size(void) const = 0;
    virtual RoStreamSize remainingSize(void) const = 0;
    virtual bool empty() const = 0;

    virtual void close(void) = 0;
};

roDEFINE_PTR(RoDataStream);

template <typename T>
auto operator << (RoDataStream& stream, const T& value) -> decltype(value.writeToStream(RoDataStream&), stream)
{
    value.writeToStream(stream);
    return stream;
}

template <typename T>
RoDataStream& operator << (RoDataStream& stream, const T& value)
{
    stream.write(&value, sizeof(T));
    return stream;
}

template <typename T>
auto operator >> (RoDataStream& stream, const T& value) -> decltype(value.readFromStream(RoDataStream&), stream)
{
    value.readFromStream(stream);
    return stream;
}

template <typename T>
RoDataStream& operator >> (RoDataStream& stream, const T& value)
{
    stream.read(&value, sizeof(T));
    return stream;
}