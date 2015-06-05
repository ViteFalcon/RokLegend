////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoFileSystem.h"
#include "RoErrorInfo.h"
#include "RoException.h"
#include "RoLog.h"
#include "RoStringUtil.h"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <sys/types.h>
#include <sys/stat.h>
//------------------------------------------------------------------------------
namespace bf = boost::filesystem;

roDEFINE_EXCEPTION(FileSystemException);
//------------------------------------------------------------------------------
RoStreamSize RoFileSystem::GetFileSize(const RoPath& filePath)
{
    if (!FileExists(filePath))
    {
        roTHROW(RoFileNotFound()
            << RoErrorInfoDetail("The requested file doesn't exist")
            << RoErrorInfoFileName(filePath.string()));
    }
    return bf::file_size(filePath);
}
//------------------------------------------------------------------------------
std::time_t RoFileSystem::GetLastUpdatedTime(const RoPath& filePath)
{
    if (!FileExists(filePath))
    {
        roTHROW(RoFileNotFound()
            << RoErrorInfoDetail("The requested file doesn't exist")
            << RoErrorInfoFileName(filePath.string()));
    }
    return bf::last_write_time(filePath);
}
//------------------------------------------------------------------------------
bool RoFileSystem::FileExists(const RoPath& filePath)
{
    return bf::exists(filePath);
}
//------------------------------------------------------------------------------
void RoFileSystem::CreateDirectories(const RoPath& directoryPath)
{
    if (!bf::is_directory(directoryPath)) {
        bf::create_directories(directoryPath);
    }
}
//------------------------------------------------------------------------------
bool RoFileSystem::FileHasExtension(const RoPath& filePath, const RoString& extension)
{
    RoString fileExtension = bf::extension(filePath);
    return boost::iequals(fileExtension, extension);
}
//------------------------------------------------------------------------------
RoPath RoFileSystem::GetWorkingDirectory()
{
    return bf::current_path();
}
//------------------------------------------------------------------------------
void RoFileSystem::ChangeWorkingDirectory(const RoPath& existingDirectory)
{
    if (!bf::is_directory(existingDirectory))
    {
        roTHROW(RoFileSystemException()
            << RoErrorInfoDetail("The specified directory does not exist.")
            << RoErrorInfoDirectory(existingDirectory.string()));
    }
    bf::current_path(existingDirectory);
}
//------------------------------------------------------------------------------
RoString GetEnvironmentValue(const RoString& environmentVariable)
{
#if roPLATFORM_IS_WIN32
    wchar_t* envValue;
    size_t length;
    errno_t error = _wdupenv_s(&envValue, &length, environmentVariable.c_str());
    if (error)
    {
        return RoStringUtil::Empty;
    }
    RoString envValueStr(envValue);
    free(envValue);
    return envValueStr;
#else
    return getenv(environmentVariable.asUTF8_c_str());
#endif
}
//------------------------------------------------------------------------------
RoPath RoFileSystem::GetGameDirectory()
{
#if roPLATFORM_IS_WIN32
    RoPath gameDir(GetEnvironmentValue("APPDATA"));
    gameDir /= "RokLegend";
    if (!bf::is_directory(gameDir))
    {
        bf::create_directories(gameDir);
    }
    return gameDir;
#elif roPLATFORM_IS_LINUX
    RoPath gameDir("~/.roklegend");
    return gameDir;
#endif
}
//------------------------------------------------------------------------------
RoString RoFileSystem::GetPathToGameDirForFile(const RoString& filename)
{
    return GetPathToGameDirForFile(filename, false);
}
//------------------------------------------------------------------------------
RoString RoFileSystem::GetPathToGameDirForFile(const RoString& filename, bool copyFromCurrent)
{
    RoPath gameFilePath = GetGameDirectory();
    gameFilePath /= filename.asWStr();
    if (!FileExists(gameFilePath) && FileExists(filename.asWStr()))
    {
        CopyFile(filename.asWStr(), gameFilePath);
        roLOG_DBG << "Copied '" << filename << "' to '" << gameFilePath.string() << "'";
    }
    return gameFilePath.native();
}
//------------------------------------------------------------------------------
RoString RoFileSystem::GetParentDirectory(const RoString& path)
{
    return RoPath(path).parent_path().native();
}
//------------------------------------------------------------------------------
RoString RoFileSystem::GetBaseName(const RoString& path)
{
    return RoPath(path).leaf().native();
}
//------------------------------------------------------------------------------
void RoFileSystem::CopyFile(const RoPath& target, const RoPath& destination)
{
    boost::filesystem::copy(target, destination);
}