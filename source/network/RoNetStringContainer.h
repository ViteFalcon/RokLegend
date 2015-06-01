////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_STRING_READ_WRITE_POLICY_H
#define ROKLEGEND_NET_STRING_READ_WRITE_POLICY_H

#include <core/RoPrerequisites.h>
#include <core/RoDataStream.h>

class RoNetData;
class RoNetPacket;
class RoNetStringContainer;
roDEFINE_PTR(RoNetData);
roDEFINE_PTR(RoNetStringContainer);

/**
  * Base class for policies that determine how strings are read from stream.
 **/
class RoNetStringContainer
{
public:
    virtual ~RoNetStringContainer();

    virtual RoNetStringContainerPtr clone(RoNetPacket& packet) const = 0;

    virtual size_t getSize() const = 0;

    virtual std::string getString() const = 0;

    virtual void assign(const std::string& val) = 0;

    virtual void readFromStream(RoDataStream& stream) = 0;
    
    virtual void writeToStream(RoDataStream& stream) const = 0;
protected:
    RoNetStringContainer();

    RoNetStringContainer(const RoNetStringContainer&) = delete;
    RoNetStringContainer& operator=(const RoNetStringContainer&) = delete;
};

class RoNetStringFixedSizeContainer : public RoNetStringContainer
{
public:
    RoNetStringFixedSizeContainer(size_t sizeOfString, const std::string& initialValue);

    virtual inline RoNetStringContainerPtr clone(RoNetPacket&) const
    {
        return RoNetStringContainerPtr(new RoNetStringFixedSizeContainer(mStringSize, mString));
    }

    virtual size_t getSize() const
    {
        return mStringSize;
    }

    virtual std::string getString() const
    {
        return mString;
    }

    virtual void assign(const std::string& val);

    virtual void readFromStream(RoDataStream& stream)
    {
        std::string data(mStringSize, 0);
        stream.read(&data[0], mStringSize);
        // make sure the last byte is NULL
        data[mStringSize-1] = 0;

        std::string stringValue(data.c_str());

        mString.swap(stringValue);
    }
    
    virtual void writeToStream(RoDataStream& stream) const
    {
        std::string data(mStringSize, 0);
        memcpy(&data[0], &mString[0], mString.length());
        stream.write(&data[0], mStringSize);
    }
private:
    const size_t mStringSize;
    std::string mString;
};

class RoNetStringNullTerminatedContainer : public RoNetStringContainer
{
public:
    RoNetStringNullTerminatedContainer(const std::string& initialValue);

    virtual inline RoNetStringContainerPtr clone(RoNetPacket&) const
    {
        return RoNetStringContainerPtr(new RoNetStringNullTerminatedContainer(mString));
    }

    virtual size_t getSize() const
    {
        return mString.length() + 1;
    }

    virtual std::string getString() const
    {
        return mString;
    }

    virtual void assign(const std::string& val)
    {
        mString = val;
    }

    virtual void readFromStream(RoDataStream& stream)
    {
        mString = stream.readUtf8();
    }
    
    virtual void writeToStream(RoDataStream& stream) const
    {
        stream.write(&mString[0], mString.length()+1);
    }

private:
    std::string mString;
};

class RoNetStringRestOfPacketContainer : public RoNetStringContainer
{
public:
    RoNetStringRestOfPacketContainer(RoNetDataPtr packetSizeField, const std::string& initialValue);

    virtual inline RoNetStringContainerPtr clone(RoNetPacket&) const;

    virtual size_t getSize() const
    {
        return mString.length() + 1;
    }

    virtual std::string getString() const
    {
        return mString;
    }

    virtual void assign(const std::string& val)
    {
        mString = val;
    }

    virtual void readFromStream(RoDataStream& stream);
    
    virtual void writeToStream(RoDataStream& stream) const;

private:
    const RoNetDataPtr mPacketSizeField;
    std::string mString;
};

#endif // ROKLEGEND_NET_STRING_READ_WRITE_POLICY_H
