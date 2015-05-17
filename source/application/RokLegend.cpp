#include <core/RoPrerequisites.h>
#include <core/RoLog.h>
#include <core/RoLogOptions.h>
#include <core/RoFileSystem.h>
#include <core/RoCpuInfo.h>

void roInitializeLogging();
void playMusic();

int main() {
    try
    {
        roInitializeLogging();
        playMusic();

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

#include <audio/RoAudioManager.h>
#include <audio/RoAudio.h>
#include <conio.h>

#define roTEST_BGM_FILE L"C:/tmp/01.mp3"

void playMusic()
{
    RoConfig config;
    RoAudioManagerPtr audioManager = RoAudioManager::Get(config);
    RoAudioPtr audio = audioManager->getBackgroundMusic(roTEST_BGM_FILE);
    _getch();
}