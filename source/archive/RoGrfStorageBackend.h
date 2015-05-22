////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_GRF_STORAGE_BACKEND_H
#define ROKLEGEND_GRF_STORAGE_BACKEND_H

#include <Core/RoPrerequisites.h>

class RoGrfArchive;
roFORWARD_DECL_PTR(RoGrfStorageBackend);
roFORWARD_DECL_PTR(RoGrfFile);

/**
  *  Interface for GRF info storage.
 **/
class RoGrfStorageBackend
{
public:
    static RoGrfStorageBackendPtr NewFromConfig();
    static RoGrfStorageBackendPtr NewFromName(const RoString& backendName);

    static RoGrfStorageBackendPtr NewHashMapStorage();
    static RoGrfStorageBackendPtr NewSqliteStorage();

    virtual ~RoGrfStorageBackend()
    {
    }

    roINLINE RoString getTypeName() const
    {
        return mTypeName;
    }

    virtual size_t getFileCountForGrf(size_t grfId) = 0;

    virtual size_t getGrfId(const RoString& grfFile) = 0;

    virtual void addFile(size_t grfId, const RoGrfFilePtr& grfFile) = 0;

    virtual bool hasFile(const RoString& grfFile) = 0;

    virtual RoGrfFilePtr getFileInfo(const RoString& grfFile) = 0;

    virtual void getFileNames(size_t grfId, RoStringArray& fileNames) = 0;

    virtual void findFileNames(size_t grfId, const RoString& pattern, RoStringArray& fileNames) = 0;

protected:
    RoGrfStorageBackend(const RoString& typeName)
        : mTypeName(typeName)
    {
    }

    const RoString mTypeName;
};

#endif // ROKLEGEND_GRF_STORAGE_BACKEND_H
