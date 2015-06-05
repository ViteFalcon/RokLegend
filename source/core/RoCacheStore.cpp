////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoCacheStore.h"
#include "RoStringUtil.h"
#include "RoFileSystem.h"

#define roCACHEDIR_X64 L"x64"
#define roCACHEDIR_X86 L"x86"
#if roARCHITECTURE_IS_64
#   define roCACHEDIR roCACHEDIR_X64
#else
#   define roCACHEDIR roCACHEDIR_X86
#endif // roARCHITECTURE_IS_64
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
RoPath roGetCacheStoreDirectory(const RoString& cacheSubDir) {
    const RoPath rootDir = RoFileSystem::GetGameDirectory() / RoPath(roCACHEDIR);
    const RoPath cacheDir = (cacheSubDir.empty() ? rootDir : rootDir / RoPath(cacheSubDir));
    RoFileSystem::CreateDirectories(cacheDir);
    return cacheDir;
}
//------------------------------------------------------------------------------
RoPath roCacheStoreFilePathForFile(const RoString& cacheDir, const RoString& fileName)
{
    return roGetCacheStoreDirectory(cacheDir) / RoPath(fileName);
}
//------------------------------------------------------------------------------
RoString roCacheStoreFileNameForFile(const RoString& cacheDir, const RoString& fileName)
{
    return RoString(roCacheStoreFilePathForFile(cacheDir, fileName).wstring());
}
//------------------------------------------------------------------------------
