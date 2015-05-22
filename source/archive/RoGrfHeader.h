////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROGRF_GRF_HEADER_H
#define ROGRF_GRF_HEADER_H

#include <Core/RoPrerequisites.h>

#pragma pack (push, 1)
struct RoGrfHeader
{
public:
    enum{
        eHeaderMagicLength = 16,
        eGrfKeyLength = 14
    };

    RoGrfHeader& fromStream(std::istream& stream);

    bool isMagicValid() const;

    bool isKeyValid() const;

    roINLINE uint32 getMajorVersion() const
    {
        return  0xFF00 & version;
    }

    roINLINE uint32 getMinorVersion() const
    {
        return 0x00FF & version;
    }

    roINLINE uint32 getVersion() const
    {
        return version;
    }

    RoString getVersionString() const;

    roINLINE uint32 getOffset() const
    {
        return offset;
    }

    roINLINE uint32 getFileCount() const
    {
        return files_count;
    }

    roINLINE uint32 getSeed() const
    {
        return seed;
    }

    bool isKeyEncoded() const;

    roINLINE friend std::istream& operator >> (std::istream& stream, RoGrfHeader& header)
    {
        header.fromStream(stream);
        return stream;
    }

private:

    uint8 magic[eHeaderMagicLength];
    uint8 key[eGrfKeyLength];

    uint32 offset;
    uint32 seed;
    uint32 files_count;
    uint32 version;
    
private: // Static
    static const std::string sHeaderMagic;
    static const uint8  sFileStatInfoSize;
    static const uint8  sKeyLength;
    static const uint8  sHeaderLength;
    static const uint8  sGrfIsEncodedKey[RoGrfHeader::eGrfKeyLength];
    static const uint8  sGrfNotEncodedKey[RoGrfHeader::eGrfKeyLength];

};
#pragma pack (pop)


#endif // ROGRF_GRF_HEADER_H
