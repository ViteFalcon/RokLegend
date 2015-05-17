////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoCpuInfo.h"
#include <sstream>
#include <boost/algorithm/string.hpp>
//-----------------------------------------------------------------------------
#define roCPUINFO_VENDOR_ID     "vendor_id"
#define roCPUINFO_MODEL_NAME    "model name"
#define roCPUINFO_FREQUENCY     "cpu MHz"
#define roCPUINFO_CACHE_SIZE    "cache size"
#define roCPUINFO_CORES         "cpu cores"
//-----------------------------------------------------------------------------
#define roCPUINFO_UNKNOWN       "[Not Known]"
//-----------------------------------------------------------------------------
const RoCpuInfo& RoCpuInfo::Get()
{
    static unique_ptr<RoCpuInfo> cpuInfo;
    static bool notInitialized = true;
    if (notInitialized)
    {
        std::string vendorId = roCPUINFO_UNKNOWN;
        std::string modelName = roCPUINFO_UNKNOWN;
        std::string cpuMhz = roCPUINFO_UNKNOWN;
        std::string cacheSize = roCPUINFO_UNKNOWN;
        std::string cpuCores = roCPUINFO_UNKNOWN;
/*#if roPLATFORM_IS_LINUX
        try
        {
            std::ifstream cpuinfo("/proc/cpuinfo", std::ios::binary | std::ios::in);
            while (!cpuinfo.eof())
            {
                std::string infoline;
                std::getline(cpuinfo, infoline);
                std::string::size_type colon_index = infoline.find(':');
                if (std::string::npos != colon_index)
                {
                    std::string keyStr = infoline.substr(0, colon_index);
                    boost::trim(keyStr);
                    if 
                    RoHashString key(keyStr.c_str());
                    if (mInfos.count(key))
                    {
                        RoString value = infoline.substr(colon_index + 1);
                        RoStringUtil::Trim(value);
                        mInfos[key] = value;
                    }
                }
            }
        }
        catch (...)
        {
            // Do nothing
        }
#endif // roPLATFORM_IS_LINUX*/
        cpuInfo.reset(new RoCpuInfo(vendorId, modelName, cpuMhz, cacheSize, cpuCores));
        notInitialized = false;
    }
    return *cpuInfo;
}
//-----------------------------------------------------------------------------
RoCpuInfo::RoCpuInfo(
    std::string vendorId,
    std::string modelName,
    std::string cpuMhz,
    std::string cacheSize,
    std::string cpuCores)
    : mVendorId(vendorId)
    , mModelName(modelName)
    , mCpuMhz(cpuMhz)
    , mCacheSize(cacheSize)
    , mCpuCores(cpuCores)
{
}
//-----------------------------------------------------------------------------
RoCpuInfo::~RoCpuInfo()
{
}
//-----------------------------------------------------------------------------
bool RoCpuInfo::isLittleEndian() const
{
#if roPLATFORM_IS_WIN32
    return true;
#elif roPLATFORM_IS_LINUX || roPLATFORM_IS_APPLE
    return !boost::algorithm::icontains(getModelName().asUTF8(), "powerpc");
#endif //
    return true;
}
//-----------------------------------------------------------------------------

RoString RoCpuInfo::printString() const
{
    std::stringstream ss;
    ss << *this;
    return RoString(ss.str());
}
//-----------------------------------------------------------------------------
