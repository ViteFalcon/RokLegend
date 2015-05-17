#include "RoVector3f.h"

namespace RoVec3f
{
    static const RoVector3f ZERO = RoVector3f::Zero();
    static const RoVector3f UNIT_X = RoVector3f::UnitX();
    static const RoVector3f UNIT_Y = RoVector3f::UnitY();
    static const RoVector3f UNIT_Z = RoVector3f::UnitZ();
    static const RoVector3f NEG_UNIT_X = -UNIT_X;
    static const RoVector3f NEG_UNIT_Y = -UNIT_Y;
    static const RoVector3f NEG_UNIT_Z = -UNIT_Z;
    static const RoVector3f UNIT = RoVector3f::Ones();
    static const RoVector3f NEG_UNIT = -UNIT;
}