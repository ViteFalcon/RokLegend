#include <core/RoPrerequisites.h>
#include <core/RoCpuInfo.h>
#include <core/RoFileSystem.h>
#include <core/RoLog.h>
#include <core/RoLogOptions.h>
#include <core/RoVariant.h>

void roInitializeLogging();
void playMusic();
void testVariants();

int main() {
    try
    {
        roInitializeLogging();
        testVariants();
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
    roLOG_INFO << "Test Value: " << testValue;
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
void testVariants()
{
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

void playMusic()
{
    RoConfig config;
    RoAudioManagerPtr audioManager = RoAudioManager::Get(config);
    RoAudioPtr audio = audioManager->getBackgroundMusic(roTEST_BGM_FILE);
    _getch();
}