#include "RoVector2f.h"

namespace RoVec2f
{
    static const RoVector2f ZERO = RoVector2f::Zero();
    static const RoVector2f UNIT_X = RoVector2f::UnitX();
    static const RoVector2f UNIT_Y = RoVector2f::UnitY();
    static const RoVector2f NEG_UNIT_X = -UNIT_X;
    static const RoVector2f NEG_UNIT_Y = -UNIT_Y;
    static const RoVector2f UNIT = RoVector2f::Ones();
    static const RoVector2f NEG_UNIT = -UNIT;
}