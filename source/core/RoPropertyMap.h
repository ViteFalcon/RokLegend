////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_PROPERTY_MAP_H
#define ROKLEGEND_PROPERTY_MAP_H

#include "RoPrerequisites.h"
#include "RoProperty.h"
#include "RoHashString.h"
#include "RoException.h"
#include "RoErrorInfo.h"

#include <boost/unordered_map.hpp>

class RoPropertyMap
{
public: // Static
    static const RoPropertyMap Empty;

public:
    enum class MergeStrategy
    {
        OVERWRITE,
        SKIP_EXISTING,
        ERROR_ON_EXISTING
    };

    RoPropertyMap(){}
    ~RoPropertyMap();

    roINLINE void swap(RoPropertyMap& other)
    {
        mProperties.swap(other.mProperties);
    }

    roINLINE void set(const RoHashString& name, const RoVariant& val)
    {
        auto entry = StrToPropertyMap::value_type(
            name,
            std::unique_ptr<RoProperty>(new RoProperty(name, val)));
        mProperties.insert(entry);
    }

    roINLINE void set(const RoProperty& property)
    {
        set(property.getHash(), property.getValue());
    }

    roINLINE bool empty() const
    {
        return mProperties.empty();
    }

    roINLINE bool has(const RoHashString& name) const
    {
        return mProperties.count(name) != 0;
    }

    roINLINE bool has(const RoString& name) const
    {
        return has(RoHashString::FromString(name));
    }

    template <typename T>
    roINLINE void set(const RoHashString &name, const T &val)
    {
        set(name, RoVariant(val));
    }

    roINLINE void remove(const RoHashString& name)
    {
        StrToPropertyMap::iterator itr = mProperties.find(name);
        if (mProperties.end() != itr)
        {
            mProperties.erase(itr);
        }
    }

    roINLINE RoProperty& getProperty(const RoHashString& name)
    {
        StrToPropertyMap::iterator itr = mProperties.find(name);
        if (mProperties.end() == itr)
        {
            roTHROW(RoItemNotFound()
                << RoErrorInfoDetail("Could not find key")
                << RoErrorInfoItemName(name.str()));
        }
        return *(itr->second);
    }

    roINLINE const RoProperty& getProperty(const RoHashString& name) const
    {
        StrToPropertyMap::const_iterator itr = mProperties.find(name);
        if (mProperties.end() == itr)
        {
            std::stringstream errorMsg;
            errorMsg<<"Item Not Found: Could not find key '"<<name<<"'!";
            throw std::runtime_error(errorMsg.str().c_str());
        }
        return *(itr->second);
    }

    RoVariant& get(const RoHashString& name)
    {
        return getProperty(name).getValue();
    }

    const RoVariant& get(const RoHashString& name) const
    {
        return getProperty(name).getValue();
    }

    template <typename T>
    RoVariant::Ref<T> get(const RoHashString& name)
    {
        return getProperty(name).as<T>();
    }

    template <typename T>
    RoVariant::ConstRef<T> get(const RoHashString& name) const
    {
        return getProperty(name).as<T>();
    }

    template <typename T>
    void get(const RoHashString &name, T& val) const
    {
        return getProperty(name).as(val);
    }

    roINLINE bool operator == (const RoPropertyMap& rhs) const
    {
        return mProperties == rhs.mProperties;
    }

    roINLINE bool operator != (const RoPropertyMap& rhs) const
    {
        return mProperties != rhs.mProperties;
    }

    void merge(const RoPropertyMap& other, MergeStrategy mergeStrategy);

private:
    using PropertyPtr = std::shared_ptr<RoProperty>;
    using StrToPropertyMap = boost::unordered_map < RoHashString, PropertyPtr > ;
    using MergeHandler = std::function < void(RoPropertyMap&, const StrToPropertyMap::mapped_type&) >;
    using MergeHandlers = boost::unordered_map < MergeStrategy, MergeHandler > ;

    StrToPropertyMap mProperties;

private: // static
    static void MergeOverwrite(RoPropertyMap& properties, const StrToPropertyMap::mapped_type& newProperty);
    static void MergeSkipExisting(RoPropertyMap& properties, const StrToPropertyMap::mapped_type& newProperty);
    static void MergeThrowIfExisting(RoPropertyMap& properties, const StrToPropertyMap::mapped_type& newProperty);

    static const MergeHandlers sMergeHandlers;
};

typedef RoVector<RoPropertyMap> RoPropertyMapArray;

#endif // ROKLEGEND_PROPERTY_MAP_H
