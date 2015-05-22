////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoCacheStore.h"
#include <core/RoFileSystem.h>
#include <core/RoStringUtil.h>
//------------------------------------------------------------------------------
bool roCacheStoreExists(RoString fileName)
{
    return RoFileSystem::FileExists(roCacheStoreFilePath(fileName).asWStr());
}
//------------------------------------------------------------------------------
RoString roCacheStoreFilePath(RoString fileName)
{
    return roCacheStoreFileNameForFile(RoStringUtil::Empty, fileName);
}
//------------------------------------------------------------------------------
RoPath roCacheStoreFilePathForFile(const RoString& cacheDir, const RoString& fileName)
{
    const RoPath gameDir = RoFileSystem::GetGameDirectory();
    const RoPath filePath = RoPath(fileName);
    if (cacheDir.empty()) {
        return gameDir / filePath;
    }
    return gameDir / RoPath(cacheDir) / filePath;
}
//------------------------------------------------------------------------------
RoString roCacheStoreFileNameForFile(const RoString& cacheDir, const RoString& fileName)
{
    return RoString(roCacheStoreFilePathForFile(cacheDir, fileName).wstring());
}
//------------------------------------------------------------------------------
