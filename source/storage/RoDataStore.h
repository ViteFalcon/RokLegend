#pragma once
#include <core/RoPrerequisites.h>

namespace sqlitepp
{
    class session;
}

enum class RoDataType
{
    None,
    Bool,
    Byte,
    Int,
    UInt,
    Float,
    String,
    Blob
};

template <typename T>
struct RoByteConverter
{
    RoBuffer toBuffer(const T& value) const
    {
        return RoBuffer{};
    }

    T fromBuffer(const RoBuffer& bytes) const
    {
        throw std::runtime_error{ "No implementation for converting bytes!" };
    }
};

class RoDataStore
{
public: // static
    static RoDataStore& GetPersistentStore();
    static RoDataStore& GetTransientStore();
public:
    RoDataStore() = default;
    virtual ~RoDataStore() = default;

    virtual sqlitepp::session& getSession() = 0;

    virtual bool hasKey(const RoString& key) = 0;

    void putInt8(const RoString& key, int8 value)
    {
        putByte(key, static_cast<uint8>(value));
    }
    void putUInt8(const RoString& key, uint8 value)
    {
        putByte(key, value);
    }
    void putInt16(const RoString& key, int16 value)
    {
        putInt(key, int64{ value });
    }
    void putUInt16(const RoString& key, uint16 value)
    {
        putUInt(key, uint64{ value });
    }
    void putInt32(const RoString& key, int32 value)
    {
        putInt(key, int64{ value });
    }
    void putUInt32(const RoString& key, uint32 value)
    {
        putUInt(key, uint64{ value });
    }
    void putInt64(const RoString& key, int64 value)
    {
        putInt(key, value);
    }
    void putUInt64(const RoString& key, uint64 value)
    {
        putInt(key, value);
    }

    template <typename T>
    void putInstance(const RoString& key, const T& value)
    {
        RoBuffer bytes = RoByteConverter<T>().toBuffer(value);
        putBlob(key, &bytes[0], bytes.size());
    }

    virtual void putBool(const RoString& key, bool value) = 0;
    virtual void putByte(const RoString& key, uint8 value) = 0;
    virtual void putInt(const RoString& key, int64 value) = 0;
    virtual void putUInt(const RoString& key, uint64 value) = 0;
    virtual void putFloat(const RoString& key, float value) = 0;
    virtual void putString(const RoString& key, const RoString& value) = 0;
    virtual void putBlob(const RoString& key, const uint8* bytes, size_t size) = 0;

    int8 getInt8(const RoString& key)
    {
        return static_cast<int8>(getByte(key));
    }
    uint8 getUInt8(const RoString& key)
    {
        return uint8{ getByte(key) };
    }

    int16 getInt16(const RoString& key)
    {
        return static_cast<int16>(getInt(key));
    }
    uint16 getUInt16(const RoString& key)
    {
        return static_cast<uint16>(getUInt(key));
    }

    int32 getInt32(const RoString& key)
    {
        return static_cast<int32>(getInt(key));
    }
    uint32 getUInt32(const RoString& key)
    {
        return static_cast<uint32>(getUInt(key));
    }

    int64 getInt64(const RoString& key)
    {
        return int64{ getInt(key) };
    }
    uint64 getUInt64(const RoString& key)
    {
        return uint64{ getUInt(key) };
    }

    std::string getUtf8String(const RoString& key)
    {
        return getString(key).asUTF8();
    }

    std::wstring getWideString(const RoString& key)
    {
        return getString(key).asWStr();
    }

    template <typename T>
    T getInstance(const RoString& key)
    {
        return RoByteConverter<T>().fromBuffer(getBlob(key));
    }

    virtual bool getBool(const RoString& key) = 0;
    virtual uint8 getByte(const RoString& key) = 0;
    virtual int64 getInt(const RoString& key) = 0;
    virtual uint64 getUInt(const RoString& key) = 0;
    virtual float getFloat(const RoString& key) = 0;
    virtual RoString getString(const RoString& key) = 0;
    virtual RoBuffer getBlob(const RoString& key) = 0;

private:
    RoDataStore(const RoDataStore&) = delete;
    RoDataStore& operator = (const RoDataStore&) = delete;
};
