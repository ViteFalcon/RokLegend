#include "RoDataStore.h"

#include <core/RoException.h>
#include <core/RoErrorInfo.h>
#include <core/RoFileSystem.h>
#include <core/RoLog.h>

#include <sqlitepp/sqlitepp.hpp>
#include <sqlitepp/statement.hpp>

namespace sqlitepp
{

    template <>
    struct converter < RoString >
    {
        typedef RoString base_type;
        static RoString to(string_t const& b)
        {
            return b;
        }
        static string_t from(RoString const& t)
        {
            return t.asUTF8();
        }
    };

    inline use_binder_ptr use(RoString const& t)
    {
        std::string utf8String = t.asUTF8();
        return use(utf8String.c_str());
    }

}

//------------------------------------------------------------------------------
// For more info on SQLite data types refer http://www.sqlite.org/datatype3.html
#define roREPO_KEY_STORE                "KeyStore"
#   define roREPO_KEY_STORE_ID          "Id"
#   define roREPO_KEY_STORE_KEYNAME     "Name"
#   define roREPO_KEY_STORE_VALUETABLE  "ValueTable"

#define roREPO_INT_STORE    "IntStore"
#define roREPO_TEXT_STORE   "TextStore"
#define roREPO_BLOB_STORE   "BlobStore"
#define roREPO_NUM_STORE    "NumStore"
#define roREPO_BOOL_STORE   roREPO_NUM_STORE
#define roREPO_CHAR_STORE   roREPO_NUM_STORE
#define roREPO_FLOAT_STORE  "RealStore"

#define roREPO_VALUE_STORE_ID       "Id"
#define roREPO_VALUE_STORE_KEYID    "Key"
#define roREPO_VALUE_STORE_VALUE    "Value"
#define roREPO_VALUE_STORE_TYPE     "UserType"
//------------------------------------------------------------------------------
class RoDataStoreImpl : public RoDataStore
{
public:
    RoDataStoreImpl(const RoString& filename)
        : mSession(filename.asUTF8())
    {
        createTables();
    }

    ~RoDataStoreImpl()
    {
        mSession.close();
    }

    virtual sqlitepp::session& getSession() override
    {
        return mSession;
    }

    virtual bool hasKey(const RoString& key) override;

    virtual void putBool(const RoString& key, bool value) override;
    virtual void putByte(const RoString& key, uint8 value) override;
    virtual void putInt(const RoString& key, int64 value) override;
    virtual void putUInt(const RoString& key, uint64 value) override;
    virtual void putFloat(const RoString& key, float value) override;
    virtual void putString(const RoString& key, const RoString& value) override;
    virtual void putBlob(const RoString& key, const uint8* bytes, size_t size) override;

    template <typename T>
    void putValue(const RoString& key, const T& val, RoDataType type);

    virtual bool getBool(const RoString& key) override;
    virtual uint8 getByte(const RoString& key) override;
    virtual int64 getInt(const RoString& key) override;
    virtual uint64 getUInt(const RoString& key) override;
    virtual float getFloat(const RoString& key) override;
    virtual RoString getString(const RoString& key) override;
    virtual RoBuffer getBlob(const RoString& key) override;

private: // types
    struct KeyInfo
    {
        uint64 id;
        RoString name;
        std::string valueTable;
    };
    using OptionalKeyInfo = optional < KeyInfo > ;
    struct ValueInfo
    {
        std::string tableName;
        uint64 id;
        uint64 keyId;
        RoDataType valueType;
    };
    using OptionalValueInfo = optional < ValueInfo > ;

private: // static methods
    static const char* ValueTableForType(RoDataType type);

private: // methods

    template <typename T>
    void getValue(const RoString& key, T& val);
    template <typename T>
    void getValue(const KeyInfo& key, T& val);

    void createTables();
    void createKeyStore();
    void createIntegerStore();
    void createTextStore();
    void createBlobStore();
    void createNumberStore();
    void createRealStore();
    void createValueStore(const char* typeName, const char* tableName);

    KeyInfo addKey(const RoString& key, const std::string& valueTable);
    void updateKey(const KeyInfo& key);
    void removeValue(const ValueInfo& value);

    OptionalValueInfo getValueInfo(const RoString& key);
    OptionalKeyInfo getKeyInfo(const RoString& key);
    OptionalValueInfo getValueInfo(const KeyInfo& keyInfo);
private: // fields
    sqlitepp::session mSession;
};

bool RoDataStoreImpl::hasKey(const RoString& key)
{
    sqlitepp::statement hasKeyStatement{ mSession };
    hasKeyStatement <<
        "SELECT "roREPO_KEY_STORE_ID" FROM "roREPO_KEY_STORE" WHERE "roREPO_KEY_STORE_KEYNAME" = :keyname;",
        sqlitepp::use(key.asUTF8());
    return hasKeyStatement.exec();
}

void RoDataStoreImpl::putBool(const RoString& key, bool value)
{
    putValue(key, value, RoDataType::Bool);
}

void RoDataStoreImpl::putByte(const RoString& key, uint8 value)
{
    putValue(key, value, RoDataType::Byte);
}

void RoDataStoreImpl::putInt(const RoString& key, int64 value)
{
    putValue(key, value, RoDataType::Int);
}

void RoDataStoreImpl::putUInt(const RoString& key, uint64 value)
{
    putValue(key, value, RoDataType::UInt);
}

void RoDataStoreImpl::putFloat(const RoString& key, float value)
{
    putValue(key, value, RoDataType::Float);
}

void RoDataStoreImpl::putString(const RoString& key, const RoString& value)
{
    putValue(key, value, RoDataType::String);
}

void RoDataStoreImpl::putBlob(const RoString& key, const uint8* bytes, size_t size)
{
    sqlitepp::blob blob = { bytes, size };
    putValue(key, blob, RoDataType::Blob);
}

template <typename T>
void RoDataStoreImpl::putValue(const RoString& key, const T& val, RoDataType type)
{
    const std::string valueTable = ValueTableForType(type);
    if (valueTable.empty())
    {
        roLOG_ERR << "Failed to store key-value pair (Key:" << key << ", Type:" << (int)type << ")";
        return;
    }
    KeyInfo keyInfo;
    OptionalKeyInfo optionalKeyInfo = getKeyInfo(key);
    if (optionalKeyInfo)
    {
        keyInfo = optionalKeyInfo.get();
        if (keyInfo.valueTable != valueTable)
        {
            keyInfo.valueTable = valueTable;
            updateKey(keyInfo);
        }
    }
    else
    {
        keyInfo = addKey(key, valueTable);
    }
    OptionalValueInfo optionalValueInfo = getValueInfo(keyInfo);
    if (optionalValueInfo)
    {
        ValueInfo valueInfo = optionalValueInfo.get();
        // Checking value type first avoids a string equality check for when
        // the types are unchanged.
        if (valueInfo.valueType != type && valueInfo.tableName != valueTable)
        {
            roLOG_INFO
                << "There has been a fundamental difference in value for key "
                "'" << key << "'"
                " from type " << (int)valueInfo.valueType <<
                " to type " << (int)type;
            removeValue(valueInfo);
        }
        else
        {
            sqlitepp::statement valueUpdateStatement(mSession);
            valueUpdateStatement <<
                "UPDATE " << valueTable << " SET "
                roREPO_VALUE_STORE_VALUE" = :value,"
                roREPO_VALUE_STORE_TYPE" = :valueType;",
                sqlitepp::use(val),
                sqlitepp::use((int)type);
            valueUpdateStatement.exec();
        }
    }
    else
    {
        sqlitepp::statement valueInsertStatement(mSession);
        valueInsertStatement <<
            "INSERT INTO " << valueTable <<
            " ("roREPO_VALUE_STORE_KEYID", "roREPO_VALUE_STORE_VALUE", "roREPO_VALUE_STORE_TYPE")"
            " VALUES(:keyid, :value, :type);",
            sqlitepp::use(keyInfo.id),
            sqlitepp::use(val),
            sqlitepp::use((int)type);
        valueInsertStatement.exec();
    }
}

bool RoDataStoreImpl::getBool(const RoString& key)
{
    bool result;
    getValue(key, result);
    return result;
}

uint8 RoDataStoreImpl::getByte(const RoString& key)
{
    uint8 result;
    getValue(key, result);
    return result;
}

int64 RoDataStoreImpl::getInt(const RoString& key)
{
    int64 result;
    getValue(key, result);
    return result;
}

uint64 RoDataStoreImpl::getUInt(const RoString& key)
{
    uint64 result;
    getValue(key, result);
    return result;
}

float RoDataStoreImpl::getFloat(const RoString& key)
{
    float result;
    getValue(key, result);
    return result;
}

RoString RoDataStoreImpl::getString(const RoString& key)
{
    sqlitepp::string_t result;
    getValue(key, result);
    return result;
}

RoBuffer RoDataStoreImpl::getBlob(const RoString& key)
{
    sqlitepp::blob blob;
    getValue(key, blob);
    RoBuffer result{};
    result.assign(blob.size, 0);
    memcpy_s(&result[0], blob.size, blob.data, blob.size);
    return result;
}

template <typename T>
void RoDataStoreImpl::getValue(const RoString& key, T& val)
{
    OptionalKeyInfo keyInfo = getKeyInfo(key);
    if (!keyInfo)
    {
        roTHROW(RoItemNotFound()
            << RoErrorInfoDetail("Failed to find the requested key!")
            << RoErrorInfoItemName(key.asUTF8()));
    }
    getValue(keyInfo.get(), val);
}

template <typename T>
void RoDataStoreImpl::getValue(const KeyInfo& key, T& val)
{
    sqlitepp::statement getValueStatement(mSession);
    getValueStatement <<
        "SELECT "roREPO_VALUE_STORE_VALUE" FROM " << key.valueTable <<
        " WHERE "roREPO_VALUE_STORE_KEYID" = :keyid;",
        sqlitepp::use(key.id),
        sqlitepp::into(val);
    if (!getValueStatement.exec())
    {
        roLOG_ERR << "Failed to retrieve value for key '" << key.name << "'";
    }
}

const char* RoDataStoreImpl::ValueTableForType(RoDataType type)
{
    switch (type)
    {
    case RoDataType::Bool:
        return roREPO_BOOL_STORE;
        break;
    case RoDataType::Byte:
        return roREPO_CHAR_STORE;
        break;
    case RoDataType::Int:
    case RoDataType::UInt:
        return roREPO_INT_STORE;
        break;
    case RoDataType::Float:
        return roREPO_FLOAT_STORE;
        break;
    case RoDataType::String:
        return roREPO_TEXT_STORE;
        break;
    case RoDataType::Blob:
        return roREPO_BLOB_STORE;
        break;
    default:
        break;
    }
    return "";
}

void RoDataStoreImpl::createTables()
{
    createKeyStore();
    createIntegerStore();
    createTextStore();
    createBlobStore();
    createNumberStore();
    createRealStore();
}

void RoDataStoreImpl::createKeyStore()
{
    sqlitepp::statement createStoreStatement(mSession);
    createStoreStatement <<
        "CREATE TABLE IF NOT EXISTS " roREPO_KEY_STORE "("
        roREPO_KEY_STORE_ID " INTEGER PRIMARY KEY,"
        roREPO_KEY_STORE_KEYNAME " TEXT UNIQUE NOT NULL,"
        roREPO_KEY_STORE_VALUETABLE " TEXT);";
    createStoreStatement.exec();
}

void RoDataStoreImpl::createIntegerStore()
{
    createValueStore("INTEGER", roREPO_INT_STORE);
}

void RoDataStoreImpl::createTextStore()
{
    createValueStore("TEXT", roREPO_TEXT_STORE);
}

void RoDataStoreImpl::createBlobStore()
{
    createValueStore("BLOB", roREPO_BLOB_STORE);
}

void RoDataStoreImpl::createNumberStore()
{
    createValueStore("NUMBER", roREPO_NUM_STORE);
}

void RoDataStoreImpl::createRealStore()
{
    createValueStore("REAL", roREPO_FLOAT_STORE);
}

void RoDataStoreImpl::createValueStore(const char* typeName, const char* tableName)
{
    sqlitepp::statement createStoreStatement(mSession);
    createStoreStatement <<
        "CREATE TABLE IF NOT EXISTS " << tableName << "("
        roREPO_VALUE_STORE_ID " INTEGER PRIMARY KEY,"
        roREPO_VALUE_STORE_KEYID " INTEGER UNIQUE NOT NULL REFERENCES " roREPO_KEY_STORE "("roREPO_KEY_STORE_ID") ON DELETE CASCADE,"
        roREPO_VALUE_STORE_VALUE " " << typeName << " NOT NULL,"
        roREPO_VALUE_STORE_TYPE" INTEGER NOT NULL);";
    createStoreStatement.exec();
}

RoDataStoreImpl::KeyInfo RoDataStoreImpl::addKey(const RoString& key, const std::string& valueTable)
{
    sqlitepp::statement insertKeyStatement(mSession);
    insertKeyStatement <<
        "INSERT INTO "roREPO_KEY_STORE
        " ("roREPO_KEY_STORE_KEYNAME","roREPO_KEY_STORE_VALUETABLE")"
        " VALUES(:keyname, :valuetable);",
        sqlitepp::use(key),
        sqlitepp::use(valueTable);
    insertKeyStatement.exec();
    return getKeyInfo(key).get();
}

void RoDataStoreImpl::updateKey(const KeyInfo& key)
{
    sqlitepp::statement updateKeyStatement(mSession);
    updateKeyStatement <<
        "UPDATE "roREPO_KEY_STORE" SET "
        roREPO_KEY_STORE_VALUETABLE" = :valuetable WHERE "roREPO_KEY_STORE_ID" = :keyid;",
        sqlitepp::use(key.valueTable),
        sqlitepp::use(key.id);
    updateKeyStatement.exec();
}

void RoDataStoreImpl::removeValue(const ValueInfo& value)
{
    sqlitepp::statement removeValueStatement(mSession);
    removeValueStatement <<
        "DELETE FROM " << value.tableName << " WHERE "
        roREPO_VALUE_STORE_ID" = :valueid;",
        sqlitepp::use(value.id);
    removeValueStatement.exec();
}

RoDataStoreImpl::OptionalValueInfo RoDataStoreImpl::getValueInfo(const RoString& key)
{
    OptionalKeyInfo optionalKeyInfo = getKeyInfo(key);
    OptionalValueInfo valueInfo;
    if (optionalKeyInfo)
    {
        valueInfo = getValueInfo(optionalKeyInfo.get());
    }
    return valueInfo;
}

RoDataStoreImpl::OptionalValueInfo RoDataStoreImpl::getValueInfo(const KeyInfo& key)
{
    sqlitepp::statement getValueTypeStatement(mSession);
    OptionalValueInfo result;
    size_t valueId = 0, valueTypeNum = 0;
    getValueTypeStatement << "SELECT "
        roREPO_VALUE_STORE_ID", "
        roREPO_VALUE_STORE_TYPE" FROM " <<
        key.valueTable << " WHERE "roREPO_VALUE_STORE_KEYID" = :keyid;",
        sqlitepp::use(key.id),
        sqlitepp::into(valueId), sqlitepp::into(valueTypeNum);
    if (getValueTypeStatement.exec())
    {
        ValueInfo value = { key.valueTable, valueId, key.id, (RoDataType)valueTypeNum };
        result = value;
    }
    return result;
}

RoDataStoreImpl::OptionalKeyInfo RoDataStoreImpl::getKeyInfo(const RoString& key)
{
    OptionalKeyInfo result;
    sqlitepp::statement getKeyIdAndValueTableStatement(mSession);
    size_t keyId = 0;
    std::string valueTable;
    getKeyIdAndValueTableStatement <<
        "SELECT "roREPO_KEY_STORE_ID","roREPO_KEY_STORE_VALUETABLE" FROM "roREPO_KEY_STORE
        " WHERE "roREPO_KEY_STORE_KEYNAME" = :keyname;",
        sqlitepp::use(key), sqlitepp::into(keyId), sqlitepp::into(valueTable);
    if (getKeyIdAndValueTableStatement.exec())
    {
        KeyInfo keyInfo = { keyId, key, valueTable };
        result = keyInfo;
    }
    return result;
}

RoDataStore& RoDataStore::GetPersistentStore()
{
    static const RoString dataStoreFile = RoFileSystem::GetPathToGameDirForFile(L"RokLegend.db");
    static RoDataStoreImpl persistentStore{ dataStoreFile };
    return persistentStore;
}

RoDataStore& RoDataStore::GetTransientStore()
{
    static const RoString memoryFilename{ L":memory:" };
    static RoDataStoreImpl transientStore{ memoryFilename };
    return transientStore;
}
