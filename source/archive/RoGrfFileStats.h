#pragma once
#ifndef ROKLEGEND_GRF_FILE_STATS_H
#define ROKLEGEND_GRF_FILE_STATS_H

#include <Core/RoPrerequisites.h>
#include <Core/RoDataStream.h>
#include <Core/RoSerialization.h>

enum RoGrfFlag
{
    eGrfFlag_File       = 1,
    eGrfFlag_MixCrypt   = 2,
    eGrfFlag_Des        = 4
};

#pragma pack(push, 1)
struct RoGrfFileStats
{
    uint32      compressedSize;
    uint32      encryptedSize;
    uint32      originalSize;
    uint8       flags;
    uint32      offset;

    roINLINE bool isEmpty() const
    {
        return 0 == originalSize;
    }

    roINLINE uint32 getOffsetAtEndOfFile() const
    {
        return offset + encryptedSize;
    }

    roINLINE bool isFile() const
    {
        return 0 != (flags & eGrfFlag_File);
    }

    roINLINE bool isEncrypted() const
    {
        return isDesEncrypted() || isMixedEncrypted();
    }

    roINLINE bool isMixedEncrypted() const
    {
        return 0 != (flags & eGrfFlag_MixCrypt);
    }

    roINLINE bool isDesEncrypted() const
    {
        return 0 != (flags & eGrfFlag_Des);
    }

    friend roINLINE std::istream& operator >> (std::istream& stream, RoGrfFileStats& fileStats)
    {
        stream.read(reinterpret_cast<char*>(&fileStats.compressedSize), 4);
        stream.read(reinterpret_cast<char*>(&fileStats.encryptedSize), 4);
        stream.read(reinterpret_cast<char*>(&fileStats.originalSize), 4);
        stream.read(reinterpret_cast<char*>(&fileStats.flags), 1);
        stream.read(reinterpret_cast<char*>(&fileStats.offset), 4);

        return stream;
    }

    friend roINLINE RoDataStream& operator >> (RoDataStream& stream, RoGrfFileStats& fileStats)
    {
        stream.read(fileStats.compressedSize);
        stream.read(fileStats.encryptedSize);
        stream.read(fileStats.originalSize);
        stream.read(fileStats.flags);
        stream.read(fileStats.offset);
        return stream;
    }

private: // serialization
    roS11N_ALLOW_PRIVATE_ACCESS;

    roS11N_SERIALIZE_FUNCTION
    {
        archive & compressedSize & encryptedSize & originalSize & flags & offset;
    }
};
#pragma pack(pop)

namespace std
{
    extern void swap(RoGrfFileStats& stats1, RoGrfFileStats& stats2);
}

#endif // ROKLEGEND_GRF_FILE_STATS_H
