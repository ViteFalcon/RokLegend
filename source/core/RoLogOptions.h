////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_LOG_OPTIONS_H
#define ROKLEGEND_LOG_OPTIONS_H

#include "RoPrerequisites.h"

enum RoLogLevel
{
    eLogLevel_None = -2,
    eLogLevel_Default = -1,
    eLogLevel_All,
    eLogLevel_Debug,
    eLogLevel_Info,
    eLogLevel_Warning,
    eLogLevel_Error,
    eLogLevel_Critical
};

struct RoLogOptions
{
public:
    static const RoLogOptions Default;

    RoLogOptions();

    RoString    logFile;
    bool        logFileAppendExisting;
    RoLogLevel  logLevel;
    bool        printIndex;
    bool        printTimestamp;
    RoString    timestampFormat;
    bool        autoAppendLine;
    bool        printToStdOut;
    bool        turnOffCache;
};

#endif // ROKLEGEND_LOG_OPTIONS_H
