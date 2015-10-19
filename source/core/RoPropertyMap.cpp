////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoPropertyMap.h"
#include "RoException.h"
#include "RoErrorInfo.h"

const RoPropertyMap RoPropertyMap::Empty;

const RoPropertyMap::MergeHandlers RoPropertyMap::sMergeHandlers = {
    {RoPropertyMap::MergeStrategy::OVERWRITE, &RoPropertyMap::MergeOverwrite},
    {RoPropertyMap::MergeStrategy::SKIP_EXISTING, &RoPropertyMap::MergeSkipExisting},
    {RoPropertyMap::MergeStrategy::ERROR_ON_EXISTING, &RoPropertyMap::MergeThrowIfExisting}
};

RoPropertyMap::~RoPropertyMap()
{
}

void RoPropertyMap::merge(const RoPropertyMap& other, MergeStrategy mergeStrategy)
{
    auto mergeHandlerItr = sMergeHandlers.find(mergeStrategy);
    roTHROW_IF(sMergeHandlers.end() == mergeHandlerItr,
        RoItemNotFound() << RoErrorInfoDetail("Failed to find the required method for requested merge-strategy!"));
    auto mergeHandler = mergeHandlerItr->second;
    for (auto propertyEntry : other.mProperties)
    {
        mergeHandler(*this, propertyEntry.second);
    }
}

void RoPropertyMap::MergeOverwrite(RoPropertyMap& properties, const StrToPropertyMap::mapped_type& newProperty)
{
    if (!newProperty)
    {
        return;
    }
    properties.mProperties[newProperty->getHash()] = newProperty;
}

void RoPropertyMap::MergeSkipExisting(RoPropertyMap& properties, const StrToPropertyMap::mapped_type& newProperty)
{
    if (!newProperty)
    {
        return;
    }
    PropertyPtr& currentEntry = properties.mProperties[newProperty->getHash()];
    if (!currentEntry)
    {
        currentEntry = newProperty;
    }
}

void RoPropertyMap::MergeThrowIfExisting(RoPropertyMap& properties, const StrToPropertyMap::mapped_type& newProperty)
{
    if (!newProperty)
    {
        return;
    }
    PropertyPtr& currentEntry = properties.mProperties[newProperty->getHash()];
    roTHROW_IF(currentEntry,
        RoDuplicateItemFound()
            << RoErrorInfoDetail("A property requested to be merged already exists")
            << RoErrorInfoItemName(newProperty->getName()));
    currentEntry = newProperty;
}

RoString to_string(const RoProperty& v)
{
    try
    {
        return v.as<RoString>();
    }
    catch (...)
    {
        return RoString("<Failed conversion to string>");
    }
}

std::ostream& operator<<(std::ostream& stream, const RoPropertyMap& properties)
{
    if (properties.empty())
    {
        return stream << "{}";
    }
    stream << "{";
    for each (auto entry in properties.mProperties)
    {
        stream << std::endl << "    " << entry.first << ": " << to_string(*entry.second) << ",";
    }
    return stream << std::endl << "}";
}
