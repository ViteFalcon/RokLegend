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
public:
    RoCpuInfo();
    virtual ~RoCpuInfo();

    RoString getVendorId() const;

    RoString getModelName() const;

    RoString getFrequency() const;

    RoString getCacheSize() const;

    RoString getCoreCount() const;

    bool isLittleEndian() const;

private: // functions
    void loadInfo();

private: // members
    typedef boost::unordered_map<RoHashString, RoString> CpuInfoMap;

    CpuInfoMap mInfos;
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
