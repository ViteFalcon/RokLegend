#pragma once
#ifndef ROKLEGEND_GRF_FILE_H
#define ROKLEGEND_GRF_FILE_H

#include <core/RoPrerequisites.h>
#include <core/RoSharedPtr.h>
#include <core/RoSerialization.h>

#include "RoGrfFileStats.h"

#include <istream>

class RoGrfFile
{
public:
    RoGrfFile(const RoString& name, const RoGrfFileStats& fileStats, void* userData = nullptr);

    ~RoGrfFile(void)
    {
    }

    roINLINE RoString getName() const
    {
        return mName;
    }

    RoString getBasename() const;

    RoString getDirectory() const;

    roINLINE uint32 getCompressedSize() const
    {
        return mFileStats.compressedSize;
    }

    roINLINE uint32 getEncryptedSize() const
    {
        return mFileStats.encryptedSize;
    }

    roINLINE uint32 getOriginalSize() const
    {
        return mFileStats.originalSize;
    }

    roINLINE bool isEncrypted() const
    {
        return mFileStats.isEncrypted();
    }

    roINLINE bool isDesEncrypted() const
    {
        return mFileStats.isDesEncrypted();
    }

    roINLINE bool isMixedEncrypted() const
    {
        return mFileStats.isMixedEncrypted();
    }

    roINLINE uint32 getOffset() const
    {
        return mFileStats.offset;
    }

    roINLINE uint32 getFlags() const
    {
        return mFileStats.flags;
    }

    roINLINE RoGrfFileStats getFileStats() const
    {
        return mFileStats;
    }

    roINLINE const void* getUserData() const
    {
        return mUserData;
    }

    roS11N_SERIALIZE_FUNCTION
    {
        archive & mName & mFileStats;
    }

private: // serialization
    roS11N_ALLOW_PRIVATE_ACCESS;

    RoGrfFile() = default;

private:
    RoString            mName;
    mutable RoString    mBasename;
    mutable RoString    mDirectory;
    RoGrfFileStats      mFileStats;
    void*               mUserData;
};

roDEFINE_PTR(RoGrfFile);

typedef std::vector<RoGrfFilePtr> RoGrfFilePtrArray;

#endif // ROKLEGEND_GRF_FILE_H
