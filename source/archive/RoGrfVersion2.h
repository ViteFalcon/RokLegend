////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROGRF_GRF_VERSION2_H
#define ROGRF_GRF_VERSION2_H

#include <core/RoPrerequisites.h>

#include "RoGrf.h"
#include "RoGrfHeader.h"

/**
  * Brief description about RoGrfVersion2.
 **/
class RoGrfVersion2 : public RoGrf
{
public:
    RoGrfVersion2(size_t grfId, const RoGrfStorageBackendPtr& store, const RoGrfHeader& header, const RoString& filename);
    virtual ~RoGrfVersion2();

private:

#pragma pack(push, 1)
    struct ContentSize
    {
        uint32 compressedSize;
        uint32 uncompressedSize;

        roINLINE bool isEmpty() const
        {
            return 0 == uncompressedSize;
        }
    };
#pragma pack(pop)

    void populateFileList();
    void populateFileListFromGrf();
    bool populateFileListFromCache();
    void saveFileListToCache();
    void validateRemainingFileSize();
    ContentSize readContentSize();
    void validateContentSizeAgainstBytesRemaining(const ContentSize& contentSize, const RoStreamSize bytesRemaining);
    RoCharArray readFileIndexContent(const ContentSize& contentSize);
    void populateFileListFromFileIndexContent(RoCharArray& fileIndexContent);
};

#endif // ROGRF_GRF_VERSION2_H
