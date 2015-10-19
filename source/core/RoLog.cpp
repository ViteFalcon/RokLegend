////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoLog.h"
#include "RoLogOptions.h"

#define BLOCK(stmts) do{stmts}while(false)
#define CHECK_AND_DO(var,stmt) if (var) { stmt; }

#if roLOG_USE_BOOST_LOG
#   include <boost/log/common.hpp>
#   include <boost/log/trivial.hpp>
#   include <boost/log/utility/setup/console.hpp>
#   include <boost/log/utility/setup/file.hpp>
#   include <boost/log/utility/setup/common_attributes.hpp>
//------------------------------------------------------------------------------
void roInitBoostLog(const RoLogOptions& options);
boost::log::trivial::severity_level roGetBoostLogLevel(const RoLogOptions& options);
//------------------------------------------------------------------------------
void roInitBoostLog(const RoLogOptions& options)
{
    namespace log = boost::log;
    namespace keywords = boost::log::keywords;
    namespace sinks = boost::log::sinks;
    boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
    log::add_common_attributes();
    boost::shared_ptr<log::core> core = log::core::get();
    core->set_filter(
        log::trivial::severity >= roGetBoostLogLevel(options)
        );
    log::add_file_log(
        keywords::file_name = options.logFile,
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
        keywords::format = "%TimeStamp%: [%Severity%] %Message%",
        keywords::auto_flush = true
        );
    log::add_console_log(std::cout, keywords::format = "%Message%");
}
//------------------------------------------------------------------------------
boost::log::trivial::severity_level roGetBoostLogLevel(const RoLogOptions& options)
{
    switch (options.logLevel)
    {
    case RoLogLevel::None:
        return boost::log::trivial::fatal;
        break;
    case RoLogLevel::All:
        return boost::log::trivial::trace;
        break;
    case RoLogLevel::Debug:
        return boost::log::trivial::debug;
        break;
    case RoLogLevel::Info:
        return boost::log::trivial::info;
        break;
    case RoLogLevel::Warning:
        return boost::log::trivial::warning;
    case RoLogLevel::Error:
        return boost::log::trivial::error;
        break;
    case RoLogLevel::Critical:
        return boost::log::trivial::fatal;
        break;
    case RoLogLevel::Default:
    default:
        break;
    };
#ifdef _DEBUG
    return boost::log::trivial::debug;
#else
    return boost::log::trivial::info;
#endif // _DEBUG
}
//------------------------------------------------------------------------------
#else
#   include <boost/logging/format.hpp>
#   include <boost/logging/writer/ts_write.hpp>

using namespace boost::logging;

BOOST_DEFINE_LOG(roLog, log_type)
BOOST_DEFINE_LOG_FILTER(roLogFilter, level::holder)
//------------------------------------------------------------------------------
void roInitBoostLogging(const RoLogOptions& options);
level::type roGetBoostLoggingLevel(const RoLogOptions& options);
//------------------------------------------------------------------------------
void roInitBoostLog(const RoLogOptions& options)
{
    // Add formatters and destinations
    // That is, how the message is to be formatted...
    CHECK_AND_DO(options.printIndex, roLog()->writer().add_formatter(formatter::idx()));
    CHECK_AND_DO(options.printTimestamp, roLog()->writer().add_formatter(formatter::time(options.timestampFormat)));
    CHECK_AND_DO(options.autoAppendLine, roLog()->writer().add_formatter(formatter::append_newline()));

    //  ... and where should it be written to
    roLog()->writer().add_destination(destination::dbg_window());
    CHECK_AND_DO(options.printToStdOut, roLog()->writer().add_destination(destination::cout()));
    if (!options.logFile.empty())
    {
        destination::file_settings settings;
        settings.do_append(options.logFileAppendExisting);
        roLog()->writer().add_destination(destination::file(options.logFile, settings));
    }
    CHECK_AND_DO(options.turnOffCache, roLog()->turn_cache_off());

    //  ... and set the log-level
    level::type boost_log_level = roGetBoostLoggingLevel(options);
    roLogFilter()->set_enabled(boost_log_level);
}
//------------------------------------------------------------------------------
level::type roGetBoostLoggingLevel(const RoLogOptions& options)
{
    switch (options.logLevel)
    {
    case eLogLevel_None:
        return level::disable_all;
        break;
    case eLogLevel_All:
        return level::enable_all;
        break;
    case eLogLevel_Debug:
        return level::debug;
        break;
    case eLogLevel_Info:
        return level::info;
        break;
    case eLogLevel_Warning:
        return level::warning;
    case eLogLevel_Error:
        return level::error;
        break;
    case eLogLevel_Critical:
        return level::fatal;
        break;
    case eLogLevel_Default:
    default:
        break;
    };
#ifdef _DEBUG
    return level::debug;
#else
    return level::info;
#endif // _DEBUG
}
//------------------------------------------------------------------------------

#endif // roLOG_USE_BOOST_LOG

//------------------------------------------------------------------------------
void roInitLogs(const RoLogOptions& options)
{
    static bool initialize = true;
    if (initialize)
    {
        roInitBoostLog(options);
        initialize = false;
    }
}
//------------------------------------------------------------------------------
void roLogInfo(const std::string& msg)
{
    roLOG_INFO << msg;
}
//------------------------------------------------------------------------------
void roLogDebug(const std::string& msg)
{
    roLOG_DBG << msg;
}
//------------------------------------------------------------------------------
void roLogWarn(const std::string& msg)
{
    roLOG_WARN << msg;
}
//------------------------------------------------------------------------------
void roLogError(const std::string& msg)
{
    roLOG_ERR << msg;
}
//------------------------------------------------------------------------------
void roLogCritical(const std::string& msg)
{
    roLOG_CRIT << msg;
}
