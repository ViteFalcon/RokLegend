////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoGrfStorageBackend.h"
#include "RoGrfSqliteBackend.h"
#include "RoGrfHashMapBackend.h"

#include <core/RoException.h>
#include <core/RoErrorInfo.h>
#include <core/RoStringUtil.h>
//------------------------------------------------------------------------------
RoGrfStorageBackendPtr RoGrfStorageBackend::NewFromConfig()
{
    //return NewFromName(RoGame::GetConfigValue<RoString>("Game.Grf.Backend", "hashmap"));
    return NewFromName("hashmap");
}
//------------------------------------------------------------------------------
RoGrfStorageBackendPtr RoGrfStorageBackend::NewFromName(const RoString& name)
{
    const RoString nameInLowerCase = RoStringUtil::ToLower(name);
    /*if (nameInLowerCase == RoGrfSqliteBackend::TypeName)
    {
        return NewSqliteStorage();
    }
    else if (nameInLowerCase == RoGrfHashMapBackend::TypeName)*/
    if (nameInLowerCase == RoGrfHashMapBackend::TypeName)
    {
        return NewHashMapStorage();
    }
    roTHROW(RoItemNotFound() << RoErrorInfoDetail("Failed to find the requested storage type.") << RoErrorInfoItemName(name));
}
//------------------------------------------------------------------------------
RoGrfStorageBackendPtr RoGrfStorageBackend::NewHashMapStorage()
{
    return std::make_shared<RoGrfHashMapBackend>();
}
//------------------------------------------------------------------------------
RoGrfStorageBackendPtr RoGrfStorageBackend::NewSqliteStorage()
{
    /*RoString sqliteFile = RoGame::GetConfigValue<RoString>(
        "Game.Grf.Backend.Sqlite.Session",
        RoGrfSqliteBackend::DefaultSession
    );
    return std::make_shared<RoGrfSqliteBackend>(sqliteFile);*/
    roTHROW(RoException{});
}
