////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoGrfVersion2.h"
#include "RoZlib.h"

#include <core/RoErrorInfo.h>
#include <core/RoException.h>
#include <core/RoFileSystem.h>
#include <core/RoLog.h>
#include <core/RoMemoryDataStream.h>
#include <core/RoStringBuilder.h>
#include <core/RoSerialization.h>
//------------------------------------------------------------------------------
#include "RoCacheStore.h"

#define roGRFV2_FILELIST_CACHE_SUFFIX L"listing.cache"
//------------------------------------------------------------------------------
RoGrfVersion2::RoGrfVersion2(size_t grfId, const RoGrfStorageBackendPtr& store, const RoGrfHeader& header, const RoString& fileName)
    : RoGrf(grfId, store, header, fileName)
{
    populateFileList();
}
//------------------------------------------------------------------------------
RoGrfVersion2::~RoGrfVersion2()
{
}
//------------------------------------------------------------------------------
roINLINE RoString roGrf2CacheFileName(size_t grfId, RoString fileName)
{
    RoStringBuilder cacheFileName;
    cacheFileName << RoFileSystem::GetBaseName(fileName) << "." << grfId << "." << roGRFV2_FILELIST_CACHE_SUFFIX;
    return cacheFileName;
}
//------------------------------------------------------------------------------
roINLINE void RoGrfVersion2::populateFileList()
{
    if (!populateFileListFromCache())
    {
        roLOG_INFO << "Failed to load file-listing from cache. It will be loaded directly from GRF.";

        populateFileListFromGrf();
        saveFileListToCache();
    }
}
//------------------------------------------------------------------------------
roINLINE void RoGrfVersion2::populateFileListFromGrf()
{
    validateRemainingFileSize();

    ContentSize contentSize = readContentSize();

    if (contentSize.isEmpty())
    {
        return;
    }

    RoStreamSize bytesRemaining = mContentSize - sizeof(ContentSize);

    validateContentSizeAgainstBytesRemaining(contentSize, bytesRemaining);

    RoCharArray fileIndexContent = readFileIndexContent(contentSize);

    populateFileListFromFileIndexContent(fileIndexContent);
}
//------------------------------------------------------------------------------
struct RoGrf2CacheHeader
{
    size_t grfId;
    RoString cachedFileName;

    bool isValidForGrf(size_t id, const RoString& grfFileName) const
    {
        return grfId == id && cachedFileName == grfFileName;
    }

    roS11N_SERIALIZE_FUNCTION
    {
        archive & grfId & cachedFileName;
    }
};
//------------------------------------------------------------------------------
struct RoGrf2FileList
{
    typedef RoVector<RoGrfFilePtr> GrfFileList;
    typedef GrfFileList::iterator Iterator;

    GrfFileList files;

    roS11N_SERIALIZE_FUNCTION
    {
        archive & files;
    }
};
//------------------------------------------------------------------------------
roINLINE bool RoGrfVersion2::populateFileListFromCache()
{
    RoString cacheFileName = roGrf2CacheFileName(mGrfId, mFileName);
    if (!roCacheStoreExists(cacheFileName))
    {
        return false;
    }

    RoCacheStore<RoInputArchive> cacheStore(cacheFileName);

    // Verify cache header
    RoGrf2CacheHeader header;
    cacheStore & header;
    if (!header.isValidForGrf(mGrfId, mFileName))
    {
        roLOG_ERR << "The cache header for GRF file-listing doesn't match the expected header!";
        return false;
    }

    RoStringBuilder cacheLoadingMessage;
    cacheLoadingMessage << "Loading GRF file-list from cache (" << cacheFileName << ") for GRF (" << mFileName << ")...";

    roLOG_INFO << cacheLoadingMessage;
    RoGrf2FileList fileList;
    cacheStore & fileList;
    for (auto file : fileList.files)
    {
        addFile(file);
    }
    roLOG_INFO << cacheLoadingMessage << " [DONE]";

    return true;
}
//------------------------------------------------------------------------------
roINLINE void RoGrfVersion2::saveFileListToCache()
{
    size_t fileCount = mStore->getFileCountForGrf(mGrfId);
    if (fileCount < 1)
    {
        // Don't save to cache if file-list is empty
        return;
    }

    RoStringArray fileNames;
    mStore->getFileNames(mGrfId, fileNames);

    RoGrf2FileList fileList;
    fileList.files.reserve(fileCount);

    for (auto fileName : fileNames)
    {
        fileList.files.push_back(mStore->getFileInfo(fileName));
    }

    RoGrf2CacheHeader header;
    header.grfId = mGrfId;
    header.cachedFileName = mFileName;

    RoCacheStore<RoOutputArchive> cacheStore(roGrf2CacheFileName(mGrfId, mFileName));
    cacheStore & header & fileList;
}
//------------------------------------------------------------------------------
roINLINE void RoGrfVersion2::validateRemainingFileSize()
{
    if (8 > mContentSize)
    {
        roTHROW(RoInvalidGrfException()
            << RoErrorInfoFileName(mFileName)
            << RoErrorInfoDetail("The file doesn't have enough information after it's header."));
    }
}
//------------------------------------------------------------------------------
roINLINE RoGrfVersion2::ContentSize RoGrfVersion2::readContentSize()
{
    ContentSize compressionSizes;
    mFileStream.read(reinterpret_cast<char*>(&compressionSizes), sizeof(ContentSize));
    return compressionSizes;
}
//------------------------------------------------------------------------------
roINLINE void RoGrfVersion2::validateContentSizeAgainstBytesRemaining(const ContentSize& contentSize, const RoStreamSize bytesRemaining)
{
    if (bytesRemaining < contentSize.compressedSize)
    {
        roTHROW(RoInvalidGrfException()
            << RoErrorInfoFileName(mFileName)
            << RoErrorInfoDetail("Unexpected end of GRF file."));
    }
}
//------------------------------------------------------------------------------
roINLINE RoCharArray RoGrfVersion2::readFileIndexContent(const ContentSize& contentSize)
{
    RoCharArray buffer(contentSize.compressedSize, 0);
    mFileStream.read(&buffer[0], contentSize.compressedSize);
    try
    {
        RoZlib::Uncompress(buffer, contentSize.uncompressedSize, buffer);
    }
    catch(RoException& exception)
    {
        exception << RoErrorInfoFileName(mFileName);
        throw;
    }
    catch(...)
    {
        throw RoInvalidGrfException()
            << RoErrorInfoFileName(mFileName)
            << RoErrorInfoDetail("Failed to uncompress file list contents.");
    }
    return buffer;
}
//------------------------------------------------------------------------------
roINLINE void RoGrfVersion2::populateFileListFromFileIndexContent(RoCharArray& fileIndexContent)
{
    typedef std::list<RoGrfFilePtr> RoGrfFilePtrList;

    RoMemoryDataStream memoryStream(&fileIndexContent[0], fileIndexContent.size(), false);
    RoDataStream& dataStream = memoryStream;

    for (uint32 i = 0; i < mHeader.getFileCount() && !dataStream.eof(); ++i)
    {
        std::string fileName = dataStream.readUtf8();
        RoGrfFileStats fileStats;
        dataStream.read(fileStats);

        fileStats.offset += sizeof(RoGrfHeader);


        if (fileStats.isEmpty() ||                          // file has no contents
            mFileSize < fileStats.getOffsetAtEndOfFile() || // file size exceeds current GRF's file size
            !fileStats.isFile())                            // it's not a file
        {
            continue;
        }

        addFile(RoGrfFilePtr(new RoGrfFile(RoString::FromEucKr(fileName), fileStats)));
    }
}
//------------------------------------------------------------------------------