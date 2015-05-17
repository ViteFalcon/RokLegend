////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_CPU_INFO_H
#define ROKLEGEND_CPU_INFO_H

#include "RoPrerequisites.h"

#include <boost/unordered_map.hpp>

/**
  * Class that loads CPU information.
 **/
class RoCpuInfo
{
public: // static
    static const RoCpuInfo& Get();
public:
    virtual ~RoCpuInfo();

    std::string getVendorId() const
    {
        return mVendorId;
    }

    std::string getModelName() const
    {
        return mModelName;
    }

    std::string getFrequency() const
    {
        return mCpuMhz;
    }

    std::string getCacheSize() const
    {
        return mCacheSize;
    }

    std::string getCoreCount() const
    {
        return mCpuCores;
    }

    bool isLittleEndian() const;

    RoString printString() const;

private: // functions
    RoCpuInfo(
        std::string vendorId,
        std::string modelName,
        std::string cpuMhz,
        std::string cacheSize,
        std::string cpuCores);

private: // members
    const std::string mVendorId;
    const std::string mModelName;
    const std::string mCpuMhz;
    const std::string mCacheSize;
    const std::string mCpuCores;
};

roINLINE std::ostream& operator << (std::ostream& stream, const RoCpuInfo& cpuInfo)
{
    return stream
        << "-------------------" << std::endl
        << "| CPU Information |" << std::endl
        << "-------------------" << std::endl
        << "Cores: " << cpuInfo.getCoreCount() << std::endl
        << "Vendor ID: " << cpuInfo.getVendorId() << std::endl
        << "Model Name: " << cpuInfo.getModelName() << std::endl
        << "Frequency (MHz): " << cpuInfo.getFrequency() << std::endl
        << "Cache Size (MB): " << cpuInfo.getCacheSize() << std::endl
        << "Is Little Endian: " << (cpuInfo.isLittleEndian()?"True":"False");
}

#endif // ROKLEGEND_CPU_INFO_H
