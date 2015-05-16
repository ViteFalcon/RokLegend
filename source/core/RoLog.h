////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_LOG_H
#define ROKLEGEND_LOG_H

#include "RoPrerequisites.h"

#define roLOG_USE_BOOST_LOG 1

#define roLOG_WHERE_EACH_LINE 0
#if defined(roLOG_WHERE_EACH_LINE) && roLOG_WHERE_EACH_LINE
#   define _roLOG_WHERE << roWHERE
#else
#   define _roLOG_WHERE
#endif

#if roLOG_USE_BOOST_LOG

#   include <boost/log/trivial.hpp>

#   define roLOG_DBG BOOST_LOG_TRIVIAL(debug)       << "[  DEBUG  ]" _roLOG_WHERE
#   define roLOG_WARN BOOST_LOG_TRIVIAL(warning)    << "[ WARNING ]" _roLOG_WHERE
#   define roLOG_ERR BOOST_LOG_TRIVIAL(error)       << "[  ERROR  ] " _roLOG_WHERE
#   define roLOG_CRIT BOOST_LOG_TRIVIAL(fatal)      << "[ CRITICAL] " _roLOG_WHERE
#   define roLOG_INFO BOOST_LOG_TRIVIAL(info)       << "[  INFO   ] "

#else

#   include <boost/logging/format_fwd.hpp>
#   include <boost/logging/writer/on_dedicated_thread.hpp>

// Optimize : use a cache string, to make formatting the message faster
BOOST_LOG_FORMAT_MSG( optimize::cache_string_one_str<> ) 

#   ifndef BOOST_LOG_COMPILE_FAST
#       include <boost/logging/format.hpp>
#       include <boost/logging/writer/ts_write.hpp>
#   endif

// Specify your logging class(es)
typedef boost::logging::logger_format_write< > log_type;

// Declare which filters and loggers you'll use
BOOST_DECLARE_LOG_FILTER(roLogFilter, boost::logging::level::holder)
BOOST_DECLARE_LOG(roLog, log_type)

// Define the macros through which you'll log
#   define roLOG_DBG BOOST_LOG_USE_LOG_IF_LEVEL(roLog(), roLogFilter(), debug ) << "[  DEBUG  ]:" _roLOG_WHERE
#   define roLOG_WARN BOOST_LOG_USE_LOG_IF_LEVEL(roLog(), roLogFilter(), warning) << "[ WARNING ]:" _roLOG_WHERE
#   define roLOG_ERR BOOST_LOG_USE_LOG_IF_LEVEL(roLog(), roLogFilter(), error ) << "[  ERROR  ]:" _roLOG_WHERE
#   define roLOG_CRIT BOOST_LOG_USE_LOG_IF_LEVEL(roLog(), roLogFilter(), fatal ) << "[ CRITICAL]:" _roLOG_WHERE
#   define roLOG_INFO BOOST_LOG_USE_LOG_IF_LEVEL(roLog(), roLogFilter(), info )

#endif // roLOG_USE_BOOST_LOG

struct RoLogOptions;

void roInitLogs(const RoLogOptions& options);

#endif // ROKLEGEND_LOG_H
