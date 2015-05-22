////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_CACHE_STORE_H
#define ROKLEGEND_CACHE_STORE_H

#include <core/RoPrerequisites.h>
#include <core/RoFileSystem.h>
#include <core/RoSerialization.h>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

typedef boost::archive::binary_iarchive RoInputArchive;
typedef boost::archive::binary_oarchive RoOutputArchive;


extern bool roCacheStoreExists(RoString fileName);
extern RoString roCacheStoreFilePath(RoString fileName);
extern RoPath roCacheStoreFilePathForFile(const RoString& cacheDir, const RoString& filename);
extern RoString roCacheStoreFileNameForFile(const RoString& cacheDir, const RoString& fileName);

template <typename T>
struct RoArchiveFileMode
{
    static int Get()
    {
        throw std::runtime_error("No file mode was specified for the requested archive type.");
    }
};

template <>
struct RoArchiveFileMode<RoInputArchive>
{
    static int Get()
    {
        return std::ios::in|std::ios::binary;
    }
};

template <>
struct RoArchiveFileMode<RoOutputArchive>
{
    static int Get()
    {
        return std::ios::out|std::ios::binary;
    }
};

template <class Archive, class ArchiveFileMode = RoArchiveFileMode<Archive>>
class RoCacheStore
{
public:
    RoCacheStore(const RoString& fileName)
        : mFilePath(roCacheStoreFilePath(fileName))
        , mStream(mFilePath.asWStr(), ArchiveFileMode::Get())
        , mArchive(mStream)
    {
    }

    virtual ~RoCacheStore()
    {
        if (mStream.is_open())
        {
            mStream.close();
        }
    }

    template <typename T>
    RoCacheStore& operator & (T& val)
    {
        mArchive & val;
        return *this;
    }

private:
    const RoString mFilePath;
    std::fstream mStream;
    Archive mArchive;
};

#endif // ROKLEGEND_CACHE_STORE_H
