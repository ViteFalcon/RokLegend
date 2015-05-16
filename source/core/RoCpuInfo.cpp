////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoCpuInfo.h"

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
RoCpuInfo::RoCpuInfo()
{
    loadInfo();
}
//-----------------------------------------------------------------------------
RoCpuInfo::~RoCpuInfo()
{
}
//-----------------------------------------------------------------------------
RoString RoCpuInfo::getVendorId() const
{
    return mInfos.at(roCPUINFO_VENDOR_ID);
}
//-----------------------------------------------------------------------------
RoString RoCpuInfo::getModelName() const
{
    return mInfos.at(roCPUINFO_MODEL_NAME);
}
//-----------------------------------------------------------------------------
RoString RoCpuInfo::getFrequency() const
{
    return mInfos.at(roCPUINFO_FREQUENCY);
}
//-----------------------------------------------------------------------------
RoString RoCpuInfo::getCacheSize() const
{
    return mInfos.at(roCPUINFO_CACHE_SIZE);
}
//-----------------------------------------------------------------------------
RoString RoCpuInfo::getCoreCount() const
{
    return mInfos.at(roCPUINFO_CORES);
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
void RoCpuInfo::loadInfo()
{
    mInfos[roCPUINFO_VENDOR_ID] = roCPUINFO_UNKNOWN;
    mInfos[roCPUINFO_MODEL_NAME] = roCPUINFO_UNKNOWN;
    mInfos[roCPUINFO_FREQUENCY] = roCPUINFO_UNKNOWN;
    mInfos[roCPUINFO_CACHE_SIZE] = roCPUINFO_UNKNOWN;
    mInfos[roCPUINFO_CORES] = roCPUINFO_UNKNOWN;

#if roPLATFORM_IS_LINUX
    try
    {
        std::ifstream cpuinfo("/proc/cpuinfo", std::ios::binary|std::ios::in);
        while (!cpuinfo.eof())
        {
            std::string infoline;
            std::getline(cpuinfo, infoline);
            std::string::size_type colon_index = infoline.find(':');
            if (std::string::npos != colon_index)
            {
                std::string keyStr = infoline.substr(0, colon_index);
                boost::trim(keyStr);
                RoHashString key(keyStr.c_str());
                if (mInfos.count(key))
                {
                    RoString value = infoline.substr(colon_index+1);
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

#endif // roPLATFORM_IS_LINUX
}
//-----------------------------------------------------------------------------
