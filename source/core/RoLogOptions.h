////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_LOG_OPTIONS_H
#define ROKLEGEND_LOG_OPTIONS_H

#include "RoPrerequisites.h"

enum class RoLogLevel
{
    None = -2,
    Default = -1,
    All,
    Debug,
    Info,
    Warning,
    Error,
    Critical
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
