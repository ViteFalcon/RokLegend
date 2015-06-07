#include "RoGrf2Impl.h"

#include <core/RoErrorInfo.h>
#include <core/RoException.h>
#include <core/RoLog.h>
#include <core/RoMemoryDataStream.h>
#include <core/RoSharedPtr.h>

#define roPROFILE_GRF_LOADING 1

#if roPROFILE_GRF_LOADING
#   include <core/RoTimer.h>
#   define roINIT_TIMER RoTimer profiler{}
#   define roPROFILE_LOG(taskMessage) roLOG_DBG << taskMessage << " in " << profiler.getMilliseconds() / 1000.0f << " secs"
#   define roTIMER_RESET profiler.reset()
#else
#   define roINIT_TIMER
#   define roPROFILE_LOG
#   define roTIMER_RESET
#endif // roPROFILE_GRF_LOADING

std::string toEucKrString(const RoString& fileName)
{
    return std::string{ utf8_to_euc_kr(fileName.asUTF8().c_str()) };
}

RoDataStreamPtr RoGrf2Impl::getFileContentsOf(const RoString& fileName) const
{
    auto fileHandle = grf_get_file(mHandle, toEucKrString(fileName).c_str());
    if (nullptr == fileHandle)
    {
        roLOG_ERR << "Failed to load contents of file: " << fileName;
        roTHROW(RoItemNotFound()
            << RoErrorInfoDetail("Failed to find the requested file.")
            << RoErrorInfoItemName(fileName));
    }
    uint32_t fileSize = grf_file_get_size(fileHandle);
    char *buffer = new char[fileSize];
    uint32_t bytesRead = grf_file_get_contents(fileHandle, buffer);
    roLOG_INFO << grf_file_get_filename(fileHandle) << ": Expected " << fileSize << " bytes. Read " << bytesRead << " bytes.";
    return std::make_shared<RoMemoryDataStream>(buffer, fileSize, true, RoMemoryDataStream::AccessMode::READ);
}

RoGrf2Ptr RoGrf2Impl::FromFile(const RoString& name)
{
    roINIT_TIMER;
    const std::string utf8Name = name.asUTF8();
    auto handle = grf_load(utf8Name.c_str(), false);
    roTHROW_IF(nullptr == handle, RoException() << RoErrorInfoDetail("Failed to load GRF!"));
    roPROFILE_LOG("Loaded GRF");
    return std::make_shared<RoGrf2Impl>(name, handle);
}

#include <core/RoGlobPattern.h>

void RoGrf2Impl::findFiles(RoStringArray& result, const RoString& pattern) const
{
    auto matcher = RoGlobPattern::New(toEucKrString(pattern));
    return filterFiles(result, [&](const std::string& fileName) {
        return matcher->matches(fileName);
    });
}

void RoGrf2Impl::filterFiles(RoStringArray& result, const RoFileNameFilter& filter) const
{
    RoVector<RoString> intermediateResults{};
    grf_node_handle* fileHandles = grf_get_file_id_list(mHandle);
    while (fileHandles && *fileHandles)
    {
        grf_node_handle currentHandle = *fileHandles++;
        std::string fileName{ grf_file_get_filename(currentHandle) };
        if (filter(fileName))
        {
            intermediateResults.push_back(RoString::FromEucKr(fileName));
        }
    }
    result.assign(intermediateResults.begin(), intermediateResults.end());
}

RoGrfFileHandle RoGrf2Impl::getFile(const RoString& name) const
{
    static const RoString prefix = "data\\wav\\";
    static const RoString ignorePrefix = "data\\wav\\effect\\";
    grf_node_handle* fileHandles = grf_get_file_id_list(mHandle);
    std::string eucKrFileName{ utf8_to_euc_kr(name.asUTF8().c_str()) };
    roLOG_DBG << "EUC-KR Filename: " << eucKrFileName.c_str();
    while (fileHandles && *fileHandles)
    {
        grf_node_handle currentHandle = *fileHandles++;
        const char* actualFilename = grf_file_get_filename(currentHandle);
        if (eucKrFileName == actualFilename)
        {
            return currentHandle;
        }
    }
    return nullptr;
}
