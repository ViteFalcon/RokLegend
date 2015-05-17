#include "RoVector3f.h"

namespace RoVec3f
{
    static const RoVector3f ZERO{ 0.0f, 0.0f, 0.0f };
    static const RoVector3f UNIT_X{ 1.0f, 0.0f, 0.0f };
    static const RoVector3f UNIT_Y{ 0.0f, 1.0f, 0.0f };
    static const RoVector3f UNIT_Z{ 0.0f, 0.0f, 1.0f };
    static const RoVector3f NEG_UNIT_X{ -1.0f, 0.0f, 0.0f };
    static const RoVector3f NEG_UNIT_Y{ 0.0f, -1.0f, 0.0f };
    static const RoVector3f NEG_UNIT_Z{ 0.0f, 0.0f, -1.0f };
    static const RoVector3f UNIT{ 1.0f, 1.0f, 1.0f };
    static const RoVector3f NEG_UNIT{ -1.0f, -1.0f, -1.0f };
}