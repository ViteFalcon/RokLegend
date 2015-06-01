////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RoPrerequisites.h"
#include "RoException.h"
#include "RoVector.h"

#include <boost/unordered_map.hpp>

/**
* A map that stores values that can be accessed via two keys.
**/
template <typename KeyType, typename ValueType, class IndexedContainerType = RoVector<ValueType> >
class RoIndexedMap
{
public:
    using IndexedContainer = IndexedContainerType;
    using Iterator = typename IndexedContainer::iterator;
    using ConstIterator = typename IndexedContainer::const_iterator;

    RoIndexedMap()
    {
    }

    ~RoIndexedMap()
    {
    }

    roINLINE bool has(const KeyType& key) const
    {
        return 0 != mKeyToIndex.count(key);
    }

    roINLINE void push_back(const KeyType& key, const ValueType& value)
    {
        mKeyToIndex[key] = mIndexedContainer.size();
        mIndexedContainer.push_back(value);
    }

    roINLINE ValueType& operator[](size_t index)
    {
        return mIndexedContainer[index];
    }

    roINLINE const ValueType& operator[](size_t index) const
    {
        return mIndexedContainer[index];
    }

    roINLINE ValueType& operator[](const KeyType& key)
    {
        const typename KeyToIndex::iterator index_itr = mKeyToIndex.find(key);
        if (mKeyToIndex.end() == index_itr)
        {
            std::stringstream itemName;
            itemName << key;
            roTHROW(RoItemNotFound()
                << RoErrorInfoDetail("Failed to find item by key")
                << RoErrorInfoItemName(itemName.str()));
        }
        return mIndexedContainer[index_itr->second];
    }

    roINLINE const ValueType& operator[](const KeyType& key) const
    {
        const typename KeyToIndex::const_iterator index_itr = mKeyToIndex.find(key);
        if (mKeyToIndex.end() == index_itr)
        {
            std::stringstream itemName;
            itemName << key;
            roTHROW(RoItemNotFound()
                << RoErrorInfoDetail("Failed to find item by key")
                << RoErrorInfoItemName(itemName.str()));
        }
        return mIndexedContainer[index_itr->second];
    }

    roINLINE Iterator begin()
    {
        return mIndexedContainer.begin();
    }

    roINLINE Iterator end()
    {
        return mIndexedContainer.end();
    }

    roINLINE ConstIterator begin() const
    {
        return mIndexedContainer.begin();
    }

    roINLINE ConstIterator end() const
    {
        return mIndexedContainer.end();
    }

    roINLINE size_t size() const
    {
        return mIndexedContainer.size();
    }
private:
    typedef boost::unordered_map<KeyType, size_t>   KeyToIndex;

    IndexedContainer mIndexedContainer;
    KeyToIndex       mKeyToIndex;
};
