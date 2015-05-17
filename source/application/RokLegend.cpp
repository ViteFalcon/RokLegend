#include <RoPrerequisites.h>
#include <RoLog.h>
#include <RoLogOptions.h>
#include <RoFileSystem.h>
#include <RoCpuInfo.h>

void roInitializeLogging();

int main() {
    try
    {
        roInitializeLogging();

        roLOG_INFO << "Starting RokLegend...";
        {
            roLOG_INFO << std::endl << RoCpuInfo::Get().printString().asUTF8();
        }
        roLOG_DBG << "Testing";
        roLOG_INFO << "Testing";
        roLOG_WARN << "Testing";
        roLOG_ERR << "Testing";
        roLOG_CRIT << "Testing";
    }
    catch (boost::exception& e)
    {
        roLOG_CRIT << boost::exception_detail::get_diagnostic_information(e, "Error");
    }
    catch (std::exception& e)
    {
        roLOG_CRIT << e.what();
    }
    catch (...)
    {
        roLOG_CRIT << "An unknown exception was caught!";
    }
    return 0;
}

void roInitializeLogging() {
    RoLogOptions logOptions;
    logOptions.logFile = RoFileSystem::GetPathToGameDirForFile("roklegend2.log");
    logOptions.logLevel = RoLogLevel::All;
    roInitLogs(logOptions);
}