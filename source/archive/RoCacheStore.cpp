////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoCacheStore.h"
#include <core/RoFileSystem.h>
#include <core/RoStringUtil.h>
#include <boost/filesystem.hpp>

#if roARCHITECTURE_IS_64
#   define roCACHE_ARCH_SUFFIX ".x64"
#else
#   define roCACHE_ARCH_SUFFIX ".x86"
#endif // roARCHITECTURE_IS_64
//------------------------------------------------------------------------------
bool roCacheStoreExists(RoString fileName)
{
    return RoFileSystem::FileExists(roCacheStoreFilePath(fileName).asWStr());
}
//------------------------------------------------------------------------------
RoString roCacheStoreFilePath(RoString fileName)
{
    const RoPath fileNamePath{ fileName };
    auto extension = fileNamePath.extension();
    auto basename = fileNamePath.stem();
    fileName = RoString{ basename.native() + roCACHE_ARCH_SUFFIX + extension.native() };
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
