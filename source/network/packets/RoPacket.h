#pragma once

#include <core/RoException.h>
#include <core/RoErrorInfo.h>
#include <core/RoVector.h>

#include "RoPacketField.h"

struct RoPacket;
roDEFINE_PTR(RoPacket);

struct RoPacket
{
    RoPacket() = default;
    virtual ~RoPacket() = default;

    virtual RoString getClassName() const = 0;
    virtual RoHashString getActionName() const = 0;
    virtual RoPropertyMap getProperties() const = 0;
    virtual void fromProperties(const RoPropertyMap& properties) = 0;
    RoPacketPtr cloneFromProperties(const RoPropertyMap& properties) const
    {
        RoPacketPtr clonedVersion = clone();
        clonedVersion->fromProperties(properties);
        return clonedVersion;
    }

    virtual RoPacketPtr clone() const = 0;

    template <typename T>
    const T& castTo() const
    {
        const T* derivedType = dynamic_cast<const T*>(this);
        roTHROW_IF(derivedType == nullptr,
            RoInvalidOperation()
                << RoErrorInfoDetail("Invalid packet conversion")
                << RoErrorInfoHint(typeid(T).name()));
        return *derivedType;
    }
};

/**
  * Base class for all translated packets. This uses CRTP to do stuff.
  *
  * NOTE: This mechanism relies on no-args constructor for packet translation from properties retrieved from raw-bytes.
  */
template <class DerivedType>
struct RoPacketT : public RoPacket
{
private:
    using PacketFieldPtr = RoSharedPtr < RoPacketField > ;
    using Fields = RoVector < PacketFieldPtr > ;

    template <typename T>
    using RequiredFieldType = typename RoClassField<DerivedType, T>::Type;
    template <typename T>
    using OptionalFieldType = typename RoClassField<DerivedType, optional<T>>::Type;
    template <typename T>
    using StructFieldType = typename RoClassField<DerivedType, RoSharedPtr<T>>::Type;
    template <typename T>
    using ListFieldType = typename RoClassField<DerivedType, RoVector<RoSharedPtr<T>>>::Type;

    static const RoString sClassName;
    static const RoHashString sActionName;
    static const size_t sRegistrationId;

    Fields mFields;

protected:
    RoPacketT() = default;
    virtual ~RoPacketT() = default;

    template <typename T>
    void add(const char* propertyName, RequiredFieldType<T> fieldPtr)
    {
        DerivedType* derivedThis = dynamic_cast<DerivedType*>(this);
        if (nullptr == derivedThis)
        {
            return;
        }
        PacketFieldPtr field{ new RoPacketFieldT<DerivedType, T>(propertyName, fieldPtr, *derivedThis) };
        mFields.push_back(field);
    }

    template <typename T>
    void addOptional(const char* propertyName, OptionalFieldType<T> fieldPtr)
    {
        DerivedType* derivedThis = dynamic_cast<DerivedType*>(this);
        if (nullptr == derivedThis)
        {
            return;
        }
        PacketFieldPtr field{ new RoPacketOptionalFieldT<DerivedType, T>(propertyName, fieldPtr, *derivedThis) };
        mFields.push_back(field);
    }

    template <typename T>
    void addStruct(const char* propertyName, StructFieldType<T> fieldPtr)
    {
        static_assert(std::is_base_of<RoPacket, T>::value, "Sub-packet should inherit from RoPacket");

        DerivedType* derivedThis = dynamic_cast<DerivedType*>(this);
        if (nullptr == derivedThis)
        {
            return;
        }
        PacketFieldPtr field = std::make_shared<RoPacketStructFieldT<DerivedType, T>>(propertyName, fieldPtr, *derivedThis);
        mFields.push_back(field);
    }

    template <typename T>
    void addList(const char* propertyName, ListFieldType<T> fieldPtr)
    {
        static_assert(std::is_base_of<RoPacket, T>::value, "List elements should inherit from RoPacket");

        DerivedType* derivedThis = dynamic_cast<DerivedType*>(this);
        if (nullptr == derivedThis)
        {
            return;
        }
        PacketFieldPtr field{ new RoPacketListFieldT<DerivedType, T>(propertyName, fieldPtr, *derivedThis) };
        mFields.push_back(field);
    }

public:
    virtual RoString getClassName() const
    {
        return sClassName;
    }

    virtual RoHashString getActionName() const
    {
        return sActionName;
    }

    virtual RoPropertyMap getProperties() const override
    {
        RoPropertyMap properties;
        for (PacketFieldPtr field : mFields)
        {
            field->write(properties);
        }
        return properties;
    }

    virtual void fromProperties(const RoPropertyMap& properties) override
    {
        for (PacketFieldPtr field : mFields)
        {
            field->read(properties);
        }
    }

    virtual RoPacketPtr clone() const
    {
        return std::make_shared<DerivedType>();
    }
};

#include "RoPacketTranslator.h"

#define roDEFINE_PACKET(PacketClassname) class PacketClassname : public RoPacketT<PacketClassname>
#define roREGISTER_PACKET(PacketName, PacketClassname) \
    const RoString RoPacketT<PacketClassname>::sClassName{ roSTRINGIFY(PacketClassname) };\
    const RoHashString RoPacketT<PacketClassname>::sActionName{ PacketName };\
    const size_t RoPacketT<PacketClassname>::sRegistrationId = RoPacketTranslator::Get().add<PacketClassname>(_H(PacketName))
#define roREGISTER_SUB_PACKET(PacketClassname) \
    const RoString RoPacketT<PacketClassname>::sClassName{ roSTRINGIFY(PacketClassname) };\
    const RoHashString RoPacketT<PacketClassname>::sActionName { roSTRINGIFY(PacketClassname) };\
    const size_t RoPacketT<PacketClassname>::sRegistrationId = -1
