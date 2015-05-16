////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoPlatform.h"
#include "RoDefines.h"

const std::string roPlatform =
#if roPLATFORM_IS_WIN32
    "Windows";
#elif roPLATFORM_IS_APPLE
    "Apple";
#elif roPLATFORM_IS_LINUX
    "Linux";
#else
    "Unknown";
#endif

const std::string roCompiler =
#if roCOMPILER_IS_MSVC
    "MSVC";
#elif roCOMPILER_IS_GNUC
    "GNU C";
#else
    "Unknown";
#endif

const std::string roCompilerVersion = roSTRINGIFY(roCOMPILER_VERSION);
