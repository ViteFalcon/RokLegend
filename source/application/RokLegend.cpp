#include <RoPrerequisites.h>
#include <RoLog.h>
#include <RoLogOptions.h>
#include <RoFileSystem.h>

void roInitializeLogging();

int main() {
    roInitializeLogging();

    roLOG_DBG << "Testing";
    roLOG_INFO << "Testing";
    roLOG_WARN << "Testing";
    roLOG_ERR << "Testing";
    roLOG_CRIT << "Testing";

    return 0;
}

void roInitializeLogging() {
    RoLogOptions logOptions;
    logOptions.logFile = RoFileSystem::GetPathToGameDirForFile("roklegend2.log");
    logOptions.logLevel = RoLogLevel::All;
    roInitLogs(logOptions);
}