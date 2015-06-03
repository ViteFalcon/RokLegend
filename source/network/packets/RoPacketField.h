#pragma once

#include <core/RoHashString.h>
#include <core/RoPropertyMap.h>
#include <core/RoVector.h>

template <typename Class, typename ValueType>
struct RoClassField
{
    typedef ValueType Class::*Type;
};

class RoPacketField
{
public:
    virtual void read(const RoPropertyMap& properties) = 0;
    virtual void write(RoPropertyMap& properties) const = 0;
};

template <class Class, typename T>
class RoPacketFieldT : public RoPacketField
{
public:
    using ValueType = T;
    typedef typename RoClassField<Class, ValueType>::Type FieldType;

    RoPacketFieldT(const char* name, FieldType field, Class& instance)
        : mName(name)
        , mField(field)
        , mInstance(instance)
    {
    }

    virtual void write(RoPropertyMap& properties) const override
    {
        auto value = getField();
        properties.set(mName, value);
    }

    virtual void read(const RoPropertyMap& properties) override
    {
        auto field = getField();
        properties.get(mName, field);
    }

private:
    ValueType& getField()
    {
        return mInstance.*mField;
    }

    const ValueType& getField() const
    {
        return mInstance.*mField;
    }

private:
    const RoHashString mName;
    FieldType mField;
    Class& mInstance;
};

template <class Class, typename T>
class RoPacketOptionalFieldT : public RoPacketField
{
public:
    using ValueType = optional < T > ;
    typedef typename RoClassField<Class, ValueType>::Type FieldType;

    RoPacketOptionalFieldT(const char* name, FieldType field, Class& instance)
        : mName(name)
        , mField(field)
        , mInstance(instance)
    {
    }
    virtual void write(RoPropertyMap& properties) const override
    {
        const ValueType& value = mInstance.*mField;
        if (value.is_initialized())
        {
            properties.set(mName, value.get());
        }
    }
    virtual void read(const RoPropertyMap& properties) override
    {
        ValueType& field = mInstance.*mField;
        if (properties.has(mName))
        {
            T value;
            properties.get(mName, value);
            field.reset(value);
        }
        else
        {
            field.reset();
        }
    }

private:
    ValueType& getField()
    {
        return mInstance.*mField;
    }

    const ValueType& getField() const
    {
        return mInstance.*mField;
    }

private:
    const RoHashString mName;
    FieldType mField;
    Class& mInstance;
};

template <class Class, typename T>
class RoPacketListFieldT : public RoPacketField
{
public:
    using PointerType = RoSharedPtr < T > ;
    using ValueType = RoVector < PointerType > ;
    using FieldType = typename RoClassField<Class, ValueType>::Type;

    RoPacketListFieldT(const char* name, FieldType field, Class& instance)
        : mName(name)
        , mField(field)
        , mInstance(instance)
    {
    }
    virtual void write(RoPropertyMap& properties) const override
    {
        const ValueType& field = mInstance.*mField;
        RoPropertyMapArray elementPropertiesList;
        elementPropertiesList.reserve(field.size());
        for (auto element : field)
        {
            elementPropertiesList.push_back(element->getProperties());
        }
        properties.set(mName, elementPropertiesList);
    }
    virtual void read(const RoPropertyMap& properties) override
    {
        ValueType list{};
        if (properties.has(mName))
        {
            RoPropertyMapArray elementProperties;
            properties.get(mName, elementProperties);
            for (auto elementProperty : elementProperties)
            {
                T value;
                auto valuePtr = value.cloneFromProperties(elementProperty);
                list.push_back(std::dynamic_pointer_cast<T>(valuePtr));
            }
        }
        ValueType& field = mInstance.*mField;
        field.swap(list);
    }

private:
    ValueType& getField()
    {
        return mInstance.*mField;
    }

    const ValueType& getField() const
    {
        return mInstance.*mField;
    }

private:
    const RoHashString mName;
    FieldType mField;
    Class& mInstance;
};
