////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoLogOptions.h"
#include "RoFileSystem.h"

const RoLogOptions RoLogOptions::Default;

RoLogOptions::RoLogOptions()
    :logFile(RoFileSystem::GetPathToGameDirForFile("RokLegend.log"))
    ,logFileAppendExisting(false)
    ,logLevel(RoLogLevel::Default)
    ,printIndex(false)
    ,printTimestamp(true)
    ,timestampFormat("[$hh:$mm.$ss] ")
    ,autoAppendLine(true)
    ,printToStdOut(false)
    ,turnOffCache(true)
{
}
