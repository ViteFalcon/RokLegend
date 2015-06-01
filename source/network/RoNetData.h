////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_DATA_H
#define ROKLEGEND_NET_DATA_H

#include <core/RoPrerequisites.h>
#include <core/RoDataStream.h>
#include <core/RoProperty.h>
#include <core/RoStringBuilder.h>

#include <sstream>

class RoNetData;
roDEFINE_PTR(RoNetData);

class RoNetPacket;

/**
  * Brief description about RoNetTypes.
 **/
class RoNetData
{
public:
    virtual ~RoNetData();

    roINLINE RoString getName() const
    {
        return mName.str();
    }

    roINLINE const RoHashString getHash() const
    {
        return mName;
    }

    virtual RoNetDataPtr clone(RoNetPacket&) const = 0;

    virtual void readFromStream(RoDataStream& stream);

    virtual void writeToStream(RoDataStream& stream) const;

    virtual size_t size() const = 0;

    bool equals(const RoNetData& other) const
    {
        if (other.mName == mName)
        {
            return equalsImpl(other);
        }
        return false;
    }

    virtual RoVariant toVariant() const = 0;
    virtual void fromVariant(const RoVariant& variant) = 0;

    virtual RoString toString() const
    {
        std::wstringstream ss;
        ss << getName() << L" : " << toVariant().toWideString();
        return ss.str();
    }

    virtual void toString(RoStringBuilder& stringBuilder) const
    {
        stringBuilder << toString();
    }

    virtual RoProperty toProperty() const
    {
        return RoProperty(mName, toVariant());
    }

    static void RewriteDataToStream(RoNetDataPtr data, RoDataStream& stream);

    roINLINE RoOptionalStreamSize _getWriteOffset() const
    {
        return mWriteOffset;
    }

    template <typename T>
    roINLINE T as() const
    {
        return toVariant().as<T>().value();
    }

    template <typename T>
    roINLINE RoNetData& operator = (const T& val)
    {
        fromVariant(RoVariant(val));
        return *this;
    }

    roINLINE bool operator == (const RoNetData& rhs) const
    {
        return equals(rhs);
    }

    template <typename T>
    roINLINE bool operator == (const T& val) const
    {
        return val == as<T>();
    }

protected:
    RoNetData(const RoHashString& name);

    virtual bool equalsImpl(const RoNetData& other) const = 0;
    virtual void readImpl(RoDataStream& stream) = 0;
    virtual void writeImpl(RoDataStream& stream) const = 0;

private:
    const RoHashString mName;
    mutable RoOptionalStreamSize mWriteOffset;
};

template <>
roINLINE RoStringBuilder& operator << (RoStringBuilder& builder, const RoNetData& data)
{
    return builder << data.toString();
}

template <>
roINLINE RoStringBuilder& operator << (RoStringBuilder& builder, const RoNetDataPtr& data)
{
    return builder << data->toString();
}

template <typename T>
class RoNetDataT : public RoNetData
{
public:
    RoNetDataT(const RoHashString& name)
        : RoNetData(name)
        , mValue(0)
    {
    }

    RoNetDataT(const RoHashString& name, const T& val)
        : RoNetData(name)
        , mValue(val)
    {
    }

    virtual size_t size() const
    {
        return sizeof(T);
    }

    virtual RoNetDataPtr clone(RoNetPacket&) const
    {
        return RoNetDataPtr(new RoNetDataT(getHash(), mValue));
    }

    virtual RoVariant toVariant() const
    {
        return RoVariant(mValue);
    }

    virtual void fromVariant(const RoVariant& val)
    {
        val.as(mValue);
    }

protected:
    virtual bool equalsImpl(const RoNetData& other) const
    {
        const RoNetDataT* rhs = dynamic_cast<const RoNetDataT*>(&other);
        if (rhs)
        {
            return rhs->mValue == mValue;
        }
        return false;
    }

    virtual void readImpl(RoDataStream& stream)
    {
        stream.read(mValue);
    }

    virtual void writeImpl(RoDataStream& stream) const
    {
        stream.write(&mValue, sizeof(mValue));
    }

    T mValue;
};

RoDataStream& operator << (RoDataStream& stream, const RoNetDataPtr& data);

RoDataStream& operator >> (RoDataStream& stream, RoNetDataPtr& data);

namespace ro
{
    namespace detail
    {
        void roWriteToDataStream(RoDataStream& stream, const RoNetDataPtr& data);

        void roReadFromDataStream(RoDataStream& stream, RoNetDataPtr& data);
    }
}

#endif // ROKLEGEND_NET_DATA_H
