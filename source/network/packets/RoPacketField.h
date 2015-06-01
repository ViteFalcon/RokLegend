#pragma once

#include <core/RoHashString.h>
#include <core/RoPropertyMap.h>

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
    typedef typename RoClassField<Class, T>::Type FieldType;

    RoPacketFieldT(const char* name, FieldType field, Class& instance)
        : name(name)
        , field(field)
        , instance(instance)
    {
    }
    virtual void write(RoPropertyMap& properties) const override
    {
        T value = instance.*field;
        properties.set(name, value);
    }
    virtual void read(const RoPropertyMap& properties) override
    {
        T value;
        properties.get(name, value);
        instance.*field = value;
    }
private:
    const RoHashString name;
    FieldType field;
    Class& instance;
};