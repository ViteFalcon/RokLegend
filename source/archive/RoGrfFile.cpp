#include "RoGrfFile.h"
#include <Core/RoFileSystem.h>

RoGrfFile::RoGrfFile(const RoString& name, const RoGrfFileStats& fileStats, void* userData)
    : mName(name)
    , mFileStats(fileStats)
    , mUserData(userData)
{
}

RoString RoGrfFile::getBasename() const
{
    if (mBasename.empty())
    {
        mBasename = RoFileSystem::GetBaseName(mName);
    }
    return mBasename;
}

RoString RoGrfFile::getDirectory() const
{
    if (mDirectory.empty())
    {
        mDirectory = RoFileSystem::GetParentDirectory(mName);
    }
    return mDirectory;
}
