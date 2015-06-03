#include <core/RoPrerequisites.h>
#include <core/RoCpuInfo.h>
#include <core/RoFileSystem.h>
#include <core/RoLog.h>
#include <core/RoLogOptions.h>
#include <core/RoTimer.h>
#include <core/RoVariant.h>
#include <core/task/RoTaskCollection.h>
#include <core/task/RoTaskManager.h>

#include <archive/RoDataFolder.h>
#include <archive/RoGrfStorageBackend.h>
#include <archive/RoGrfFactory.h>
#include <archive/RoGrf.h>

#include <audio/RoAudioManager.h>
#include <audio/RoAudio.h>

#include <network/RoNetworkManager.h>

#include <storage/RoDataInfo.h>

#include <conio.h>
#include <fstream>

#define roROOT_FOLDER_FILE "ROOT"

//#define roGRF_TEST_FILE "C:\\Users\\ullatil\\Games\\Ragnarok Online\\data.grf"
#define roTEST_BGM_FILE L"C:/tmp/01.mp3"
#define roGRF_TEST_FILE "C:\\Users\\ullatil\\Games\\InertiaRO\\data.grf"
#define roKEY_ESC 27
#define roKEY_RETURN 13

void roInitializeLogging();
void roChangeRootFolder();
void testVariants(const RoTaskArgs& args);
void mainLoop(const RoTaskArgs& args);
void loginPrompt(const RoTaskArgs& args);

roDEFINE_PTR(RoNetworkManager);


enum class RoCliGameState
{
    NONE,
    LOGIN_PROMPT,
    LOGIN_REQUEST_SENT,
    LOGIN_SUCCEEDED,
    LOGIN_FAILED
};
using RoAtomicCliGameState = RoAtomic < RoCliGameState > ;
roDEFINE_PTR(RoAtomicCliGameState);

roDEFINE_TASK_ARGS(RoLoginPromptEvent)
{
    RoNetworkManagerPtr networkManager;
    RoAtomicCliGameStatePtr gameState;
};

int main() {
    try
    {
        roInitializeLogging();
        roChangeRootFolder();

        roLOG_INFO << "Starting RokLegend...";
        {
            roLOG_INFO << std::endl << RoCpuInfo::Get().printString().asUTF8();
        }
        roLOG_DBG << "Testing";
        roLOG_INFO << "Testing";
        roLOG_WARN << "Testing";
        roLOG_ERR << "Testing";
        roLOG_CRIT << "Testing";

        RoTaskCollection tasks;
        tasks.add("RunVariantTests", testVariants);
        tasks.add("MainLoop", mainLoop);
        tasks.add("LoginPrompt", loginPrompt);

        roSCHEDULE_TASK(RunVariantTests, RoEmptyArgs::INSTANCE);
        roRUN_TASK(MainLoop, RoEmptyArgs::INSTANCE);
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

void mainLoop(const RoTaskArgs& args)
{
    RoConfig config;
    RoAudioManagerPtr audioManager = RoAudioManager::Get(config);
    RoAtomicCliGameStatePtr gameState{ new RoAtomicCliGameState{ RoCliGameState::NONE } };

    RoAudioPtr audio = audioManager->getBackgroundMusic(roTEST_BGM_FILE);
    audio->setIsPaused(false);

    RoTimer profiler{};
    auto grf = RoGrf2::FromFile(roGRF_TEST_FILE);
    roLOG_DBG << "Took " << profiler.getMilliseconds() / 1000.0f << " seconds to load GRF";
    profiler.reset();
    RoStringArray files = grf->findFiles("*\\ef_teleportation.wav");
    roLOG_DBG << "Found " << files.size() << " file(s) in " << profiler.getMilliseconds() / 1000.0f << " seconds";
    for (RoString fileName : files)
    {
        roLOG_DBG << "\t" << fileName;
    }
    roLOG_DBG << "--- " << RoDataInfo::Get().getValue("dir:Sfx", "NOTFOUND");
    RoString testSound = files[0];
    auto soundDataStream = grf->getFileContentsOf(testSound);
    auto sound = audioManager->getSound2D(testSound, soundDataStream, false);

    RoNetworkManagerPtr networkManager = std::make_shared<RoNetworkManager>("data/packets.xml");

    std::cout << "Ready to accept inputs!" << std::endl;
    audioManager->playSound2D(testSound, false);
    char ch = 0;
    do
    {
        ch = kbhit() ? _getch() : 0;
        roSCHEDULE_TASK(_NetworkUpdate, RoEmptyArgs::INSTANCE);
        if (roKEY_RETURN == ch)
        {
            RoCliGameState expectedState = RoCliGameState::NONE;
            if (gameState->compare_exchange_strong(expectedState, RoCliGameState::LOGIN_PROMPT))
            {
                audioManager->playSound2D(testSound, false);
                RoLoginPromptEvent event;
                event.networkManager = networkManager;
                event.gameState = gameState;
                roRUN_TASK(LoginPrompt, event);
            }
            else
            {
                std::cerr << "Found unexpected state: " << as_integer(expectedState) << std::endl;
            }
        }
        Sleep(0);
    } while (ch != roKEY_ESC);
}

#include <core/RoHashSet.h>

std::string readPassword(const std::string& message)
{
    RoHashSet<char> allowedChars = { '.', ',', '!', '@', '$', '%', '^', '&', '*', '+', '-', '_', '=' };
    RoVector<char> password;
    std::cout << message;
    char ch = 0;
    do 
    {
        ch = _getch();
        if (isalnum(ch) || allowedChars.count(ch))
        {
            password.push_back(ch);
            std::cout << "*";
        }
    } while (ch != roKEY_RETURN);
    std::cout << std::endl;
    password.push_back(0);
    std::string passwordString(password.begin(), password.end());
    return passwordString;
}

void loginPrompt(const RoTaskArgs& args)
{
    auto loginPromptState = RoCliGameState::LOGIN_PROMPT;
    auto loginEvent = as_event_args<RoLoginPromptEvent>(args);
    if (loginEvent.gameState->load() != loginPromptState)
    {
        return;
    }
    std::string username;
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::string password = readPassword("Enter Password: ");
    if (loginEvent.gameState->compare_exchange_strong(loginPromptState, RoCliGameState::NONE))
    {
        std::cout << "TODO: Send login event via network manager." << std::endl;
        return;
    }
    std::cerr << "Failed to login due to unexpected game-state '" << as_integer(loginPromptState) << "'" << std::endl;
}

void roInitializeLogging() {
    RoLogOptions logOptions;
    logOptions.logFile = RoFileSystem::GetPathToGameDirForFile("roklegend2.log");
    logOptions.logLevel = RoLogLevel::All;
    roInitLogs(logOptions);
}

void roChangeRootFolder()
{
    RoPath rootPathFilePath = RoFileSystem::GetWorkingDirectory() / RoPath{ roROOT_FOLDER_FILE };
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