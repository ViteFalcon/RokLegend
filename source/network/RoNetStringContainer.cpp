////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoNetStringContainer.h"
#include "RoNetPacket.h"

roDEFINE_EXCEPTION(InvalidStringSize);

RoNetStringContainer::RoNetStringContainer()
{
}

RoNetStringContainer::~RoNetStringContainer()
{
}

RoNetStringFixedSizeContainer::RoNetStringFixedSizeContainer(size_t sizeOfString, const std::string& initialValue)
    : mStringSize(sizeOfString)
{
    if (sizeOfString < 2)
    {
        roTHROW(RoInvalidStringSize()
            << RoErrorInfoDetail("String size of a RoNetString should be greater than 1.")
            << RoErrorInfoHint("If you want to store values of 1, use RoNetByte instead.")
        );
    }
    assign(initialValue);
}

void RoNetStringFixedSizeContainer::assign(const std::string& val)
{
    std::string stringValue;
    if (!val.empty())
    {
        size_t stringSize = roMIN(mStringSize-1, val.length());
        stringValue.resize(stringSize, 0);
        memcpy(&stringValue[0], &val[0], stringSize);
    }
    mString.swap(stringValue);
}

RoNetStringNullTerminatedContainer::RoNetStringNullTerminatedContainer(const std::string& initialValue)
    : mString(initialValue)
{
}

RoNetStringRestOfPacketContainer::RoNetStringRestOfPacketContainer(RoNetDataPtr packetSizeField, const std::string& initialValue)
    : mPacketSizeField(packetSizeField)
    , mString(initialValue)
{
}

RoNetStringContainerPtr RoNetStringRestOfPacketContainer::clone(RoNetPacket& packet) const
{
    return RoNetStringContainerPtr(new RoNetStringRestOfPacketContainer(packet.getField(mPacketSizeField->getHash()), mString));
}

void RoNetStringRestOfPacketContainer::readFromStream(RoDataStream& stream)
{
    size_t bytesToRead = mPacketSizeField->as<size_t>() - size_t(stream.bytesRead());
    std::string string(bytesToRead+1, 0);
    stream.read(&string[0], bytesToRead);
    mString = std::string(string.c_str());
}

void RoNetStringRestOfPacketContainer::writeToStream(RoDataStream& stream) const
{
    stream.write(&mString[0], mString.length()+1);
    RoOptionalStreamSize packetSizeFieldOffset = mPacketSizeField->_getWriteOffset();
    uint32 streamPos = uint32(stream.tell());
    mPacketSizeField->fromVariant(RoVariant(streamPos));
    RoNetData::RewriteDataToStream(mPacketSizeField, stream);
}
