////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoGrfVersion1.h"

#include <core/RoErrorInfo.h>
#include <core/RoException.h>
#include <core/RoFileSystem.h>
#include <core/RoMemoryDataStream.h>
//------------------------------------------------------------------------------
typedef std::fstream::streampos RoFileStreamPos;
//------------------------------------------------------------------------------
RoGrfVersion1::RoGrfVersion1(size_t grfId, const RoGrfStorageBackendPtr& store, const RoGrfHeader& header, const RoString& fileName)
    : RoGrf(grfId, store, header, fileName)
{
    populateFileList();
}
//------------------------------------------------------------------------------
RoGrfVersion1::~RoGrfVersion1()
{
}
//------------------------------------------------------------------------------
void RoGrfVersion1::populateFileList()
{
    validateRemainingFileSize();

    populateFileListFromFileStream();
}
//------------------------------------------------------------------------------
roINLINE void RoGrfVersion1::validateRemainingFileSize()
{
    if (!mContentSize)
    {
        roTHROW(RoInvalidGrfException()
            << RoErrorInfoDetail("Reached end of GRF before getting to file-table.")
            << RoErrorInfoFileName(mFileName));
    }
}
//------------------------------------------------------------------------------
roINLINE void RoGrfVersion1::populateFileListFromFileStream()
{
    typedef std::list<RoGrfFilePtr> RoGrfFilePtrList;
    RoGrfFilePtrList fileList;

    RoCharArray buffer((size_t)mContentSize, '\0');
    mFileStream.read(&buffer[0], std::streamsize(mContentSize));

    size_t fileIndex = 0;
    RoMemoryDataStream memoryStream(&buffer[0], (size_t)mContentSize, false);
    RoDataStream& dataStream = memoryStream;
    while(!dataStream.eof())
    {
        uint32 encodedFileNameLength;
        dataStream.read(encodedFileNameLength);
        encodedFileNameLength -= 2;
        
        uint16 unknownBytes;
        dataStream.read(unknownBytes);
        (void)unknownBytes;

        RoCharArray encodedFileName(encodedFileNameLength, '\0');
        dataStream.read(&encodedFileName[0], encodedFileNameLength);
        
        RoString fileName = decodeFileName(encodedFileName);

        RoGrfFileStats fileStats;
        dataStream.read(fileStats);

        // Do some adjustments to make the stats right
        // This is PURE mathemagic
        fileStats.encryptedSize -= 37579;
        fileStats.compressedSize += fileStats.originalSize + 715;

        fileStats.offset += sizeof(RoGrfHeader);

        fileStats.flags |= isDesEncryptedFile(fileName) ? eGrfFlag_Des : eGrfFlag_MixCrypt;

        if (fileStats.isEmpty() ||                          // file has no contents
            mFileSize < fileStats.getOffsetAtEndOfFile() || // file size exceeds current GRF's file size
            !fileStats.isFile())                            // it's not a file
        {
            continue;
        }

        addFile(RoGrfFilePtr(new RoGrfFile(fileName, fileStats)));
    }
}
//------------------------------------------------------------------------------
roINLINE bool RoGrfVersion1::isDesEncryptedFile(const RoString& fileName)
{
    return
        RoFileSystem::FileHasExtension(fileName.asWStr(), "gnd") ||
        RoFileSystem::FileHasExtension(fileName.asWStr(), "gat") ||
        RoFileSystem::FileHasExtension(fileName.asWStr(), "act") ||
        RoFileSystem::FileHasExtension(fileName.asWStr(), "str");
}
//------------------------------------------------------------------------------