////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////


#include "RoNetServerType.h"

class RoNetServerTypeInfo
{
public:
    RoNetServerTypeInfo(RoNetServerType type, const RoString& typeString)
        : mType(type)
        , mTypeString(typeString)
    {
    }

    virtual ~RoNetServerTypeInfo()
    {
    }

    RoNetServerType getType() const
    {
        return mType;
    }

    RoString toString() const
    {
        return mTypeString;
    }

    virtual bool fromString(const RoString& str) const
    {
        return str == toString();
    }

private:
    const RoNetServerType mType;
    const RoString mTypeString;
};

class RoNoneServerTypeInfo: public RoNetServerTypeInfo
{
public:
    RoNoneServerTypeInfo()
        : RoNetServerTypeInfo(RoNetServerType::NONE, "None")
    {
    }
};

class RoLoginServerTypeInfo : public RoNetServerTypeInfo
{
public:
    RoLoginServerTypeInfo()
        : RoNetServerTypeInfo(RoNetServerType::LOGIN, "LoginServer")
    {
    }
};

class RoCharacterServerTypeInfo : public RoNetServerTypeInfo
{
public:
    RoCharacterServerTypeInfo()
        : RoNetServerTypeInfo(RoNetServerType::CHARACTER, "CharacterServer")
    {
    }
};

class RoMapServerTypeInfo : public RoNetServerTypeInfo
{
public:
    RoMapServerTypeInfo()
        : RoNetServerTypeInfo(RoNetServerType::MAP, "MapServer")
    {
    }
};

class RoNetServerTypeInfoFactory
{
public:
    static RoNetServerTypeInfo& Get(int type)
    {
        return Get(RoNetServerType(type));
    }

    static RoNetServerTypeInfo& Get(RoNetServerType type)
    {
        static RoNoneServerTypeInfo none;
        static RoLoginServerTypeInfo login;
        static RoCharacterServerTypeInfo character;
        static RoMapServerTypeInfo map;

        switch (type)
        {
        case RoNetServerType::LOGIN:
            return login;
        case RoNetServerType::CHARACTER:
            return character;
        case RoNetServerType::MAP:
            return map;
        }
        return none;
    }
};

RoString roNetServerTypeToString(RoNetServerType type)
{
    RoNetServerTypeInfo& serverTypeInfo = RoNetServerTypeInfoFactory::Get(type);
    return serverTypeInfo.toString();
}

RoNetServerType roNetServerTypeFromString(const RoString &serverTypeString)
{
    for (int type = as_integer(RoNetServerType::LOGIN); type < as_integer(RoNetServerType::COUNT); ++type)
    {
        RoNetServerTypeInfo& serverTypeInfo = RoNetServerTypeInfoFactory::Get(type);
        if (serverTypeInfo.fromString(serverTypeString))
        {
            return serverTypeInfo.getType();
        }
    }
    return RoNetServerType::NONE;
}
