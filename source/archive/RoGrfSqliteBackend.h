////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_GRF_SQLITE_BACKEND_H
#define ROKLEGEND_GRF_SQLITE_BACKEND_H

#include <core/RoPrerequisites.h>
#include "RoGrfStorageBackend.h"
#include "RoGrfFile.h"

//#include <sqlitepp/sqlitepp.hpp>

/**
  * Brief description about RoGrfSqliteBackend.
 **/
/*class RoGrfSqliteBackend : public RoGrfStorageBackend
{
public:
    static const RoString TypeName;
    static const RoString MemorySession;
    static const RoString FileSession;
    static const RoString DefaultSession;

    RoGrfSqliteBackend(const RoString& sessionType);
    virtual ~RoGrfSqliteBackend();

    roINLINE RoString getSessionType() const
    {
        return mSessionType;
    }

    roINLINE bool isMemorySessionType() const
    {
        return mSessionType == MemorySession;
    }

    roINLINE bool isFileSessionType() const
    {
        return mSessionType == FileSession;
    }

    size_t getGrfId(const RoString& grfFile);

    size_t getFileCountForGrf(size_t grfId);

    void addFile(size_t grfId, const RoGrfFilePtr& grfFile);

    bool hasFile(const RoString& grfFile);

    bool hasFileLike(const RoString& pattern);

    RoGrfFilePtr getFileInfo(const RoString& grfFile);

    void getFileNames(size_t grfId, Ogre::StringVector& fileNames);

    void getFileInfos(RoGrfArchive* archive, Ogre::FileInfoList& fileInfos);

    void findFileNames(size_t grfId, const RoString& pattern, Ogre::StringVector& fileNames);

    void findFileInfos(const RoGrfArchive* archive, const RoString& pattern, Ogre::FileInfoList& fileInfos);

private: // Functions
    static RoString ResolveSessionType(const RoString& sessionType);
    static sqlitepp::session& GetSession(const RoString& sessionType);

    void createTables();
    void createGrfIdTable();
    void createFilesTable();

private: // Members
    const RoString mSessionType;
    sqlitepp::session& mSession;
};
*/

#endif // ROKLEGEND_GRF_SQLITE_BACKEND_H
