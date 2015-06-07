#include <core/RoPrerequisites.h>
#include <core/RoCpuInfo.h>
#include <core/RoFileSystem.h>
#include <core/RoLog.h>
#include <core/RoLogOptions.h>
#include <core/RoTimer.h>
#include <core/RoVariant.h>
#include <core/message_queue/RoMessageQueue.h>
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
#define roSTR_BUTTON_SOUND "str:ButtonSound"

//#define roGRF_TEST_FILE "C:\\Users\\ullatil\\Games\\Ragnarok Online\\data.grf"
#define roTEST_BGM_FILE L"C:/tmp/01.mp3"
#define roGRF_TEST_FILE "C:\\Users\\ullatil\\Games\\InertiaRO\\data.grf"
//C:\Users\ullatil\Games\EsunaRO
//#define roGRF_TEST_FILE "C:\\Users\\ullatil\\Games\\Ragnarok Online\\cdata.grf"
#define roKEY_ESC 27
#define roKEY_RETURN 13


enum class RoCliGameState
{
    NONE,
    LOGIN_SERVER_READY,
    LOGIN_PROMPT,
    LOGIN_REQUEST_SENT,
    LOGIN_SUCCEEDED,
    LOGIN_FAILED
};
using RoAtomicCliGameState = RoAtomic < RoCliGameState > ;
roDEFINE_PTR(RoAtomicCliGameState);
RoAtomicCliGameStatePtr gGameState{ new RoAtomicCliGameState{ RoCliGameState::NONE } };

void roInitializeLogging();
void roChangeRootFolder();
void testVariants(const RoTaskArgs& args);
void mainLoop(const RoTaskArgs& args);
void connectToLoginServer();
void loginServerConnectFailed(const RoTaskArgs& args);
void loginServerConnected(const RoTaskArgs& args);
void loginServerDisconnected(const RoTaskArgs& args);
void loginPrompt(const RoTaskArgs& args);
void loginSuccessful(const RoTaskArgs& args);
void loginFailed(const RoTaskArgs& args);
bool changeGameState(RoCliGameState& expectedState, const RoCliGameState newState);
RoCliGameState getGameState();

roDEFINE_PTR(RoNetworkManager);

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
        tasks.add("AccountServerInfo", loginSuccessful);
        tasks.add("LoginServerConnectionFailed", loginServerConnectFailed);
        tasks.add("LoginServerConnected", loginServerConnected);
        tasks.add("LoginServerDisconnected", loginServerDisconnected);

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
    RoString buttonSoundFile = RoDataInfo::Get().getValue(roSTR_BUTTON_SOUND, "NOTFOUND");
    RoString testSound = "buttonSound.wav";
    auto soundDataStream = grf->getFileContentsOf(buttonSoundFile);
    auto sound = audioManager->getSound2D(testSound, soundDataStream, false);

    RoNetworkManagerPtr networkManager = std::make_shared<RoNetworkManager>("data/packets.xml");

    std::cout << "Ready to accept inputs!" << std::endl;
    audioManager->playSound2D(testSound, false);
    connectToLoginServer();
    RoMessageQueue& messageQueue = RoMessageQueue::Get(RoMessageQueue::eMessageQueue_Default);
    char ch = 0;
    do
    {
        messageQueue.dispatch();
        ch = kbhit() ? _getch() : 0;
        roSCHEDULE_TASK(_NetworkUpdate, RoEmptyArgs::INSTANCE);
        if (roKEY_RETURN == ch)
        {
            audioManager->playSound2D(testSound, false);
        }
        Sleep(0);
    } while (ch != roKEY_ESC);
}

bool changeGameState(RoCliGameState& expectedState, const RoCliGameState newState)
{
    const RoCliGameState expected = expectedState;
    if (!gGameState->compare_exchange_strong(expectedState, newState))
    {
        std::cerr << "Unexpected game-state found. Current: "
            << as_integer(expectedState)
            << ", Transition: " << as_integer(expected) << " -> " << as_integer(newState)
            << std::endl;
        return false;
    }
    return true;
}

RoCliGameState getGameState()
{
    return gGameState->load();
}

void loginServerConnectFailed(const RoTaskArgs& args)
{
    RoCliGameState currentState = RoCliGameState::NONE;
    changeGameState(currentState, RoCliGameState::NONE);
    std::cerr << "Failed to connect to login server" << std::endl;
}

void loginServerConnected(const RoTaskArgs& args)
{
    RoCliGameState currentState = RoCliGameState::NONE;
    if (changeGameState(currentState, RoCliGameState::LOGIN_SERVER_READY))
    {
        std::cout << "Connected to login server..." << std::endl;
        RoLoginPromptEvent event;
        roRUN_TASK(LoginPrompt, event);
    }
}

void loginServerDisconnected(const RoTaskArgs& args)
{
    std::cerr << "Disconnected from login server." << std::endl;
}

#include <network/events/RoServerConnectRequestEvent.h>

void connectToLoginServer()
{
    RoNetworkManager::Connect(RoNetServerType::LOGIN, "127.0.0.1", "6900");
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

#include <network/events/RoSendPacketToServerEvent.h>
#include <network/packets/RoLoginCredentials.h>

void loginPrompt(const RoTaskArgs& args)
{
    auto currentState = RoCliGameState::LOGIN_SERVER_READY;
    auto loginPromptState = RoCliGameState::LOGIN_PROMPT;
    auto loginEvent = as_event_args<RoLoginPromptEvent>(args);
    if (!changeGameState(currentState, loginPromptState))
    {
        return;
    }
    std::string username;
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::string password = readPassword("Enter Password: ");
    if (!changeGameState(loginPromptState, RoCliGameState::LOGIN_REQUEST_SENT))
    {
        return;
    }
    auto loginPacket = std::make_shared<RoLoginCredentials>();
    loginPacket->setUsername(username);
    loginPacket->setPassword(password);
    RoNetworkManager::SendToServer(RoNetServerType::LOGIN, loginPacket);
}

void loginSuccessful(const RoTaskArgs& args)
{
    std::cout << "Login succeeded!" << std::endl;
    RoCliGameState currentState = RoCliGameState::LOGIN_REQUEST_SENT;
    changeGameState(currentState, RoCliGameState::LOGIN_SUCCEEDED);
    RoNetworkManager::Disconnect(RoNetServerType::LOGIN);
}

void loginFailed(const RoTaskArgs& args)
{
    std::cout << "Login FAILED!" << std::endl;
    RoCliGameState currentState = RoCliGameState::LOGIN_REQUEST_SENT;
    changeGameState(currentState, RoCliGameState::LOGIN_FAILED);
    RoNetworkManager::Disconnect(RoNetServerType::LOGIN);
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