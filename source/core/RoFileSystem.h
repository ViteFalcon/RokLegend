////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_FILESYSTEM_H
#define ROKLEGEND_FILESYSTEM_H

#include "RoPrerequisites.h"

#include <boost/filesystem/path.hpp>

typedef boost::filesystem::path RoPath;

class RoFileSystem
{
public:
    static RoStreamSize GetFileSize(const RoPath& filePath);

    static std::time_t GetLastUpdatedTime(const RoPath& filepath);

    static bool FileExists(const RoPath& filePath);

    static bool FileHasExtension(const RoPath& filePath, const RoString& extension);

    static RoPath GetWorkingDirectory();

    static void ChangeWorkingDirectory(const RoPath& existingDirectory);

    static RoPath GetGameDirectory();

    static RoString GetPathToGameDirForFile(const RoString& filename);

    static RoString GetPathToGameDirForFile(const RoString& filename, bool copyFromCurrent);

    static RoString GetParentDirectory(const RoString& path);

    static RoString GetBaseName(const RoString& path);

    static void CopyFile(const RoPath& target, const RoPath& destination);
};

#endif // ROKLEGEND_FILESYSTEM_H
