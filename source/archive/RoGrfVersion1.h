////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROGRF_GRF_VERSION1_H
#define ROGRF_GRF_VERSION1_H

#include <Core/RoPrerequisites.h>

#include <Archive/RoGrf.h>
#include <Archive/RoGrfHeader.h>

/**
  * Brief description about RoGrfVersion1.
 **/
class RoGrfVersion1 : public RoGrf
{
public:
    RoGrfVersion1(size_t grfId, const RoGrfStorageBackendPtr& store, const RoGrfHeader& header, const RoString& fileName);
    virtual ~RoGrfVersion1();

private:
    void populateFileList();
    void validateRemainingFileSize();
    void populateFileListFromFileStream();
    bool isDesEncryptedFile(const RoString& fileName);
};

#endif // ROGRF_GRF_VERSION1_H
