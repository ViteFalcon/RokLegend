#include "RokLegend.h"

#include <core/RoCpuInfo.h>
#include <core/RoFileSystem.h>
#include <core/RoLog.h>
#include <core/RoLogOptions.h>
#include <core/RoTimer.h>
#include <core/message_queue/RoMessageQueue.h>
#include <core/task/RoTaskManager.h>

#include <archive/RoGrf.h>
#include <audio/RoAudioManager.h>
#include <network/RoNetworkManager.h>

#include <storage/RoDataInfo.h>

#include "RoGameBindings.h"
#include "gamestates/RoGameState.h"

const RoString RokLegend::EXIT_TASK{ L"_ExitGame" };

RokLegend::RokLegend(
    RoAudioManagerPtr audioManager,
    RoNetworkManagerPtr networkManager,
    RoGameStateFactoryPtr gameStateFactory)
    : mAudioManager(audioManager)
    , mNetworkManager(networkManager)
    , mGameStateFactory(gameStateFactory)
    , mGameState{ RoGameStates::NONE }
    , mNewGameState{ RoGameStates::NONE }
    , mCanRunGame{ false }
{
    addTaskHandler(EXIT_TASK, &RokLegend::stopGame);
}

void RokLegend::run()
{
    bool expectedCanRunState = false;
    if (!mCanRunGame.compare_exchange_strong(expectedCanRunState, true))
    {
        roLOG_CRIT << "Cannot run game as it seems like a game-loop is already running.";
        return;
    }

    {
        RoTimer profiler{};
        auto grf = RoGameBindings::getGrf();
        roLOG_DBG << "Took " << profiler.getMilliseconds() / 1000.0f << " seconds to load GRF";
        profiler.reset();

        RoStringArray files = grf->findFiles("*\\ef_teleportation.wav");
        roLOG_DBG << "Found " << files.size() << " file(s) in " << profiler.getMilliseconds() / 1000.0f << " seconds";
        for (RoString fileName : files)
        {
            roLOG_DBG << "\t" << fileName;
        }
    }

    setGameState(RoGameStates::LOGIN);

    RoMessageQueue& messageQueue = RoMessageQueue::Get(RoMessageQueue::eMessageQueue_Default);
    RoTimer deltaTimer;
    RoGameStatePtr gameState;
    do
    {
        const RoGameStates newGameState = mNewGameState.load();
        if (newGameState != mGameState)
        {
            gameState = mGameStateFactory->getGameState(newGameState);
            mGameState = newGameState;
        }

        auto deltaSeconds = deltaTimer.getMilliseconds() / 1000.0f;
        RoNetworkManager::ScheduleUpdate();
        messageQueue.dispatch();
        gameState->update(deltaSeconds);
        // FIXME: Get rid of this sleep once moved to graphics
        Sleep(0);
    } while (mCanRunGame.load());
}

void RokLegend::setGameState(RoGameStates gameState)
{
    mNewGameState.store(gameState);
}

void RokLegend::exit() const
{
    roRUN_TASK_NAMED(EXIT_TASK, RoEmptyArgs::INSTANCE);
}

void RokLegend::stopGame(const RoTaskArgs& args)
{
    mCanRunGame.store(false);
}

void roInitializeLogging();
void roChangeRootFolder();

int main() {
    try
    {
        roInitializeLogging();
        roChangeRootFolder();
        RokLegendPtr game = RoGameBindings::getGame();
        game->run();
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

void roInitializeLogging()
{
    RoLogOptions logOptions;
    logOptions.logFile = RoFileSystem::GetPathToGameDirForFile("roklegend2.log");
    logOptions.logLevel = RoLogLevel::All;
    roInitLogs(logOptions);
    roLOG_INFO << "Starting RokLegend...";
    {
        roLOG_INFO << std::endl << RoCpuInfo::Get().printString().asUTF8();
    }
}

void roChangeRootFolder()
{
    static const RoPath ROOT_FOLDER_FILE{ "ROOT" };
    RoPath rootPathFilePath = RoFileSystem::GetWorkingDirectory() / ROOT_FOLDER_FILE;
    if (!RoFileSystem::FileExists(rootPathFilePath))
    {
        roLOG_DBG << "No root folder defining file was found.";
        return;
    }
    std::string rootPathString;

    std::ifstream rootPathFile(rootPathFilePath.string().c_str(), std::ios::in | std::ios::binary);
    rootPathFile >> rootPathString;
    rootPathFile.close();
    if (RoFileSystem::FileExists(rootPathString))
    {
        roLOG_DBG << "Changing directory to " << rootPathString;
        RoFileSystem::ChangeWorkingDirectory(rootPathString);
    }
    else
    {
        roLOG_ERR << "Failed to change directory to '" << rootPathString << "', since it doesn't exist.";
    }
}

class VariantTestLogger
{
public:
    VariantTestLogger(const char* testName)
        : mTestName(testName)
    {
        roLOG_INFO << "############### START OF TEST: " << testName << " ###############";
    }
    ~VariantTestLogger()
    {
        roLOG_INFO << "-------------------------END OF TEST----------------------------\n";
    }

    void startTestCase(const char* testCaseName) const
    {
        roLOG_INFO << "Test Case: " << testCaseName;
    }

private:
    const char* mTestName;
};

template <typename TestTypeFrom, typename TestTypeTo>
void variantTestCaseForTypes(const VariantTestLogger& log, const char* testCaseName, const TestTypeFrom testValue)
{
    try
    {
        log.startTestCase(testCaseName);
        RoVariant fromVariant{ testValue };
        TestTypeTo toType = fromVariant.as<TestTypeTo>();
        roLOG_INFO << "    Value: " << toType;
    }
    catch (const boost::exception& ex)
    {
        roLOG_ERR << "Failed test case: " << testCaseName << boost::exception_detail::get_diagnostic_information(ex, "....");
    }
    catch (const std::exception& ex)
    {
        roLOG_ERR << "Failed test case: " << testCaseName << ex.what();
    }
    catch (...)
    {
        roLOG_CRIT << "Unknown exception type thrown!";
    }
}

template <typename TestType>
void variantTestForType(const char* testName, const TestType testValue)
{
    VariantTestLogger log{ testName };
    RoVariant variant{ testValue };
    roLOG_INFO << "Test Value: " << testValue << " (stored as " << variant.getType().name() << ")";
    variantTestCaseForTypes<TestType, char>(log, "To Char", testValue);
    variantTestCaseForTypes<TestType, uint8>(log, "To uint8", testValue);
    variantTestCaseForTypes<TestType, int16>(log, "To int16", testValue);
    variantTestCaseForTypes<TestType, uint16>(log, "To uint16", testValue);
    variantTestCaseForTypes<TestType, int32>(log, "To int32", testValue);
    variantTestCaseForTypes<TestType, uint32>(log, "To uint32", testValue);
    variantTestCaseForTypes<TestType, int64>(log, "To int64", testValue);
    variantTestCaseForTypes<TestType, uint64>(log, "To uint64", testValue);
    variantTestCaseForTypes<TestType, float>(log, "To float", testValue);
    variantTestCaseForTypes<TestType, double>(log, "To double", testValue);
    variantTestCaseForTypes<TestType, std::string>(log, "To string", testValue);
    variantTestCaseForTypes<TestType, std::wstring>(log, "To wstring", testValue);
}
void testVariants(const RoTaskArgs& args)
{
    roLOG_INFO << "Task ID: " << args.taskId;
    // -- int
    variantTestForType<int8>("int8", 65);
    variantTestForType<uint8>("uint8", 65);
    variantTestForType<int16>("int16", 65);
    variantTestForType<uint16>("int16", 65);
    variantTestForType<int32>("int32", 65);
    variantTestForType<uint32>("int32", 65);
    variantTestForType<int64>("int64", 65);
    variantTestForType<uint64>("int64", 65);
    variantTestForType<float>("float", 65.5432871098f);
    variantTestForType<double>("double", 65.5432871098);
    variantTestForType<std::string>("std::string", "66");
    variantTestForType<std::wstring>("std::wstring", L"66");
}
