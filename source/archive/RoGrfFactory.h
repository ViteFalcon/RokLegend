////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROGRF_GRF_FACTORY_H
#define ROGRF_GRF_FACTORY_H

#include <Core/RoPrerequisites.h>
#include <Core/RoSharedPtr.h>

struct RoGrfHeader;

roFORWARD_DECL_PTR(RoGrf);
roFORWARD_DECL_PTR(RoGrfStorageBackend);

/**
  * Factory class to generate GRF reader instances.
 **/
class RoGrfFactory
{
public:
    RoGrfFactory(RoGrfStorageBackendPtr store);
    virtual ~RoGrfFactory();

    static bool IsGrfVersionSupported(uint32 version);

    RoGrfPtr getGrf(const RoString& grfFileName);

private:
    RoGrfPtr createGrfInstance(const RoGrfHeader& header, const RoString& grfFileName);
    RoGrfStorageBackendPtr mStore;
};

#endif // ROGRF_GRF_FACTORY_H
