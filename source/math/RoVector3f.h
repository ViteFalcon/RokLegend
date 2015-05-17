////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2015
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Eigen/Dense"

using RoVector3f = Eigen::Vector3f;

namespace RoVec3f
{
    extern const RoVector3f ZERO;
    extern const RoVector3f UNIT_X;
    extern const RoVector3f UNIT_Y;
    extern const RoVector3f UNIT_Z;
    extern const RoVector3f NEG_UNIT_X;
    extern const RoVector3f NEG_UNIT_Y;
    extern const RoVector3f NEG_UNIT_Z;
    extern const RoVector3f UNIT;
    extern const RoVector3f NEG_UNIT;
}