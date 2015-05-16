////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_PROPERTY_H
#define ROKLEGEND_PROPERTY_H

#include "RoPrerequisites.h"
#include "RoVariant.h"
#include "RoHashString.h"

class RoProperty
{
public:

    roINLINE RoProperty(const RoHashString& name)
        : mName(name)
    {
    }

    roINLINE RoProperty(const RoHashString &name, const RoVariant& value)
        : mName(name)
        , mValue(value)
    {
    }

    roINLINE RoProperty(const RoProperty& other)
        : mName(other.mName)
        , mValue(other.mValue)
    {
    }

    roINLINE virtual void swap(RoProperty& other)
    {
        mName.swap(other.mName);
        mValue.swap(other.mValue);
    }

    roINLINE RoProperty& operator = (const RoProperty& rhs)
    {
        RoProperty(rhs).swap(*this);
        return *this;
    }

    template <typename T>
    roINLINE RoProperty& operator = (const T& rhs)
    {
        mValue = rhs;
        return *this;
    }

    roINLINE const RoHashString& getHash() const
    {
        return mName;
    }

    roINLINE RoString getName() const
    {
        return mName.str();
    }

    roINLINE RoVariant& getValue()
    {
        return mValue;
    }

    roINLINE const RoVariant& getValue() const
    {
        return mValue;
    }

    template <typename T>
    roINLINE void setValue(const T& rhs)
    {
        mValue = rhs;
    }

    roINLINE void setValue(const RoVariant& rhs)
    {
        mValue = rhs;
    }

    template <typename T>
    roINLINE RoVariant::Ref<T> as()
    {
        return mValue.as<T>();
    }

    template <typename T>
    roINLINE RoVariant::ConstRef<T> as() const
    {
        return mValue.as<T>();
    }

    template <typename T>
    roINLINE void as(T& val) const
    {
        mValue.as(val);
    }

    virtual RoProperty* clone() const
    {
        return new RoProperty(mName, mValue);
    }

private:
    RoHashString mName;
    RoVariant mValue;
};

#endif // ROKLEGEND_PROPERTY_H
