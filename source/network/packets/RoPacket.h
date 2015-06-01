#pragma once

#include "RoPacketField.h"

struct RoPacket
{
    RoPacket() = default;
    virtual ~RoPacket() = default;

    virtual RoPropertyMap getProperties() const = 0;
    virtual void fromProperties(const RoPropertyMap& properties) = 0;
};

template <class DerivedType>
struct RoPacketT : public RoPacket
{
private:
    using PacketFieldPtr = RoSharedPtr < RoPacketField > ;
    using Fields = RoVector < PacketFieldPtr > ;

    Fields mFields;

protected:
    RoPacketT() = default;
    virtual ~RoPacketT() = default;

    template <typename T>
    void add(const char* propertyName, typename RoClassField<DerivedType, T>::Type fieldPtr)
    {
        DerivedType* derivedThis = dynamic_cast<DerivedType*>(this);
        if (nullptr == derivedThis)
        {
            return;
        }
        PacketFieldPtr field{ new RoPacketFieldT<DerivedType, T>(propertyName, fieldPtr, *derivedThis) };
        mFields.push_back(field);
    }

public:
    RoPropertyMap getProperties() const override
    {
        RoPropertyMap properties;
        for (PacketFieldPtr field : mFields)
        {
            field->write(properties);
        }
        return properties;
    }

    void fromProperties(const RoPropertyMap& properties) override
    {
        for (PacketFieldPtr field : mFields)
        {
            field->read(properties);
        }
    }
};
