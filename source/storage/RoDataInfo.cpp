#include "RoDataInfo.h"
#include "RoDataStore.h"

#include <core/RoException.h>
#include <core/RoErrorInfo.h>
#include <core/RoHashMap.h>
#include <core/RoHashSet.h>
#include <core/RoLog.h>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <sqlitepp/sqlitepp.hpp>

class RoDataInfoImpl : public RoDataInfo
{
public: // static
    using KeyValueMap = RoHashMap < RoString, RoString > ;
    using UniqueStrings = RoHashSet < std::string > ;

private: // static
    static const boost::regex PlaceHolderRegex;

public:
    static void LoadDataIntoMap(RoDataStore& dataFile, KeyValueMap& map);
    static void ResolveStringsInMap(KeyValueMap& map);
    static RoString ResolveString(const KeyValueMap& map, const RoString& key, const RoString& val, UniqueStrings unresolvedKeys = UniqueStrings{});

public:
    RoDataInfoImpl(KeyValueMap keyValues)
        : mKeyValues(keyValues)
    {
    }

    virtual RoOptionalString getValue(const RoString& key) const override;

private:
    KeyValueMap mKeyValues;
};
//------------------------------------------------------------------------------
#define roDATAINFO_TABLE "StringData"
const boost::regex RoDataInfoImpl::PlaceHolderRegex("\\{[A-Za-z0-9.:]*\\}");
//------------------------------------------------------------------------------
void RoDataInfoImpl::LoadDataIntoMap(RoDataStore& dataStore, KeyValueMap& map)
{
    sqlitepp::session& dataSession = dataStore.getSession();
    sqlitepp::string_t key, val;
    sqlitepp::statement selectAllDataStrings(dataSession);
    selectAllDataStrings << "SELECT Name, Value FROM " << roDATAINFO_TABLE << ";",
        sqlitepp::into(key), sqlitepp::into(val);
    while (selectAllDataStrings.exec())
    {
        if (map.count(key))
        {
            roLOG_ERR << "The key '" << key << "' has already been defined!";
            continue;
        }
        map[key] = val;
    }
}

void RoDataInfoImpl::ResolveStringsInMap(KeyValueMap& map)
{
    KeyValueMap::iterator keyValueItr = map.begin(), keyValueEnd = map.end();
    while (keyValueEnd != keyValueItr)
    {
        keyValueItr->second = ResolveString(map, keyValueItr->first, keyValueItr->second);
        ++keyValueItr;
    }
}

RoString RoDataInfoImpl::ResolveString(const KeyValueMap& map, const RoString& key, const RoString& val, UniqueStrings unresolvedKeys /*= UniqueStrings()*/)
{
    const std::string utf8Key = key.asUTF8();
    if (unresolvedKeys.count(utf8Key))
    {
        roTHROW(RoInvalidOperation() << RoErrorInfoDetail("Found a cyclic dependency in data-info!") << RoErrorInfoItemName(utf8Key));
    }
    std::string utfVal = val.asUTF8();
    std::string resolvedVal = utfVal;
    boost::sregex_token_iterator tokenItr(utfVal.begin(), utfVal.end(), PlaceHolderRegex, 0);
    const boost::sregex_token_iterator tokenEnd;
    while (tokenEnd != tokenItr)
    {
        std::string dependentKey(tokenItr->first + 1, tokenItr->second - 1);
        if (!map.count(dependentKey))
        {
            roTHROW(RoItemNotFound()
                << RoErrorInfoDetail("Failed to find the referenced key in data-info value!")
                << RoErrorInfoItemName(dependentKey)
                << RoErrorInfoHint(utfVal));
        }
        if (unresolvedKeys.count(dependentKey))
        {
            roTHROW(RoInvalidOperation()
                << RoErrorInfoDetail("Found a cyclic dependency in data-info!")
                << RoErrorInfoItemName(utf8Key)
                << RoErrorInfoHint(dependentKey));
        }
        unresolvedKeys.insert(dependentKey);
        std::string dependentValue = ResolveString(map, key, map.at(dependentKey), unresolvedKeys);
        unresolvedKeys.erase(dependentKey);
        static const std::string BracketStart = "{";
        static const std::string BracketEnd = "}";
        boost::algorithm::replace_all(resolvedVal, BracketStart + dependentKey + BracketEnd, dependentValue);
        ++tokenItr;
    }
    return resolvedVal;
}

RoOptionalString RoDataInfoImpl::getValue(const RoString& key) const
{
    RoOptionalString result;
    if (mKeyValues.count(key))
    {
        result = mKeyValues.at(key);
    }
    return result;
}

RoDataInfo& RoDataInfo::Get()
{
    static RoSharedPtr<RoDataInfo> instance{};
    if (!instance)
    {
        RoDataInfoImpl::KeyValueMap keyValues{};
        RoDataInfoImpl::LoadDataIntoMap(RoDataStore::GetPersistentStore(), keyValues);
        RoDataInfoImpl::ResolveStringsInMap(keyValues);
        instance.reset(new RoDataInfoImpl{ keyValues });
    }
    return *instance;
}

RoString RoDataInfo::getValue(const RoString& key, const RoString& defaultValue)
{
    RoOptionalString optionalValue = getValue(key);
    return optionalValue.get_value_or(defaultValue);
}
