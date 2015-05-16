////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoHashString.h"

namespace{
    static size_t CalculateFNV(const char* str)
    {
        size_t hash = roFNV_OFFSET;
        while (*str != 0)
        {
            hash = (hash ^ *str++) * roFNV_PRIME;
        }
        return hash;
    }
}
//------------------------------------------------------------------------------
RoHashString::RoHashString(ConstCharWrapper str)
:mHash(CalculateFNV(str.str))
#if roHASHSTRING_STORE_STRING
,mString(str.str)
#endif // _DEBUG
{
}
//------------------------------------------------------------------------------
RoHashString::~RoHashString()
{
}
