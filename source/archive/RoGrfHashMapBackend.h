////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_GRF_VECTOR_BACKEND_H
#define ROKLEGEND_GRF_VECTOR_BACKEND_H

#include <Core/RoPrerequisites.h>
#include <Archive/RoGrfStorageBackend.h>

#include <boost/unordered_map.hpp>

roFORWARD_DECL_PTR(RoGlobPattern);

/**
  * GRF backend using C++ data-structures.
 **/
class RoGrfHashMapBackend : public RoGrfStorageBackend
{
public:
    static const RoString TypeName;

    RoGrfHashMapBackend();
    virtual ~RoGrfHashMapBackend();

    virtual size_t getFileCountForGrf(size_t grfId);

    virtual size_t getGrfId(const RoString& grfFile);

    virtual void addFile(size_t grfId, const RoGrfFilePtr& grfFile);

    virtual bool hasFile(const RoString& grfFile);

    virtual RoGrfFilePtr getFileInfo(const RoString& grfFile);

    virtual void getFileNames(size_t grfId, RoStringArray& fileNames);

    virtual void findFileNames(size_t grfId, const RoString& pattern, RoStringArray& fileNames);

private:
    struct FileInfoFilter
    {
        virtual bool operator()(const RoGrfFile& fileInfo) const = 0;
    };

    struct NoFilter : public FileInfoFilter
    {
        virtual bool operator()(const RoGrfFile& fileInfo) const
        {
            return true;
        }
    };

    struct GlobFilter : public FileInfoFilter
    {
        GlobFilter(const std::string& pattern);
        virtual bool operator()(const RoGrfFile& fileInfo) const;
        RoGlobPatternPtr matcher;
    };

    virtual void findFileNames(size_t grfId, RoStringArray& fileNames, const FileInfoFilter& filter);

    typedef boost::unordered_map<RoString, RoGrfFilePtr> GrfHashMap;
    typedef RoVector<GrfHashMap> GrfArray;
    typedef boost::unordered_map<RoString, size_t> GrfNameIndexMap;

    GrfArray mGrfs;
    GrfNameIndexMap mNameToIndex;
};

#endif // ROKLEGEND_GRF_VECTOR_BACKEND_H
