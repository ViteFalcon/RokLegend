#pragma once
#include <core/RoSharedPtr.h>
#include <core/RoVector.h>

roFORWARD_DECL_PTR(RoCharacterServer);

using RoCharacterServerCollection = RoVector < RoCharacterServerPtr > ;
roDEFINE_PTR(RoCharacterServerCollection);