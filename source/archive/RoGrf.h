#pragma once
#ifndef ROKLEGEND_GRF_H
#define ROKLEGEND_GRF_H

#include <core/RoPrerequisites.h>
#include <core/RoDataStream.h>
#include <core/RoFileSystem.h>

#include "RoGrfFile.h"
#include "RoGrfHeader.h"
#include "RoGrfStorageBackend.h"

#include <fstream>

class RoGrfFileCacheInfo;

class RoGrf
{
public: // Functions
    using FileIterator = RoGrfFilePtrArray::iterator;
    using FileConstIterator = RoGrfFilePtrArray::const_iterator;

    ~RoGrf();

    roINLINE size_t getId() const
    {
        return mGrfId;
    }

    roINLINE RoString getName() const
    {
        return mFileName;
    }

    roINLINE size_t fileCount()
    {
        return mStore->getFileCountForGrf(mGrfId);
    }

    roINLINE bool hasFile(const RoString& fileName)
    {
        return mStore->hasFile(fileName);
    }

    roINLINE RoString getVersionString() const
    {
        return mHeader.getVersionString();
    }

    RoDataStreamPtr getFileContentsOf(const RoString& fileName);

    roINLINE RoGrfFilePtr getFileInfo(const RoString& file)
    {
        return mStore->getFileInfo(file);
    }

protected: // Functions
    using OptionalDataStreamPtr = optional<RoDataStreamPtr>;
    roDEFINE_PTR(RoGrfFileCacheInfo);
    RoGrf(size_t grfId, const RoGrfStorageBackendPtr& store, const RoGrfHeader& header, const RoString& fileName);

    roINLINE void addFile(RoGrfFilePtr file)
    {
        mStore->addFile(mGrfId, file);
    }

    RoString decodeFileName(const RoCharArray& encodedFileName);

    bool hasSpecialExtension(const RoString& file);

    OptionalDataStreamPtr getCachedFileContents(const RoString& fileName);

    RoPath getCachedFileName(const RoString& fileName);

protected:

    const size_t            mGrfId;
    const RoGrfHeader       mHeader;
    const RoStreamSize      mFileSize;
    const std::time_t       mLastUpdateTime;
    const RoStreamSize      mContentSize;
    RoString                mFileName;
    std::fstream            mFileStream;
    uint32                  mHeaderSize;
    RoGrfStorageBackendPtr  mStore;
};

roFORWARD_DECL_PTR(RoGrf2);

using RoFileNameFilter = std::function < bool(const std::string&) > ;

class RoGrf2
{
public: // static
    static RoGrf2Ptr FromFile(const RoString& fileName);

public:
    virtual ~RoGrf2() = default;

    inline RoString getName() const
    {
        return mName;
    }

    virtual RoDataStreamPtr getFileContentsOf(const RoString& fileName) const = 0;

    inline RoStringArray findFiles(const RoString& pattern) const
    {
        RoStringArray result;
        findFiles(result, pattern);
        return result;
    }

    inline RoStringArray filterFiles(const RoFileNameFilter& filter) const
    {
        RoStringArray result;
        filterFiles(result, filter);
        return result;
    }
protected:
    RoGrf2(const RoString& name)
        : mName(name)
    {
    }
    virtual void findFiles(RoStringArray& result, const RoString& pattern) const = 0;
    virtual void filterFiles(RoStringArray& result, const RoFileNameFilter& filter) const = 0;

private:
    const RoString mName;
};

#endif // ROKLEGEND_GRF_H
