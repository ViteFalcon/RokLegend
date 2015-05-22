////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoGrfFactory.h"
#include "RoGrfErrorInfo.h"
#include "RoGrfVersion1.h"
#include "RoGrfVersion2.h"

#include <Core/RoErrorInfo.h>
#include <Core/RoException.h>
#include <Core/RoFileSystem.h>

#include <fstream>

namespace {

    roINLINE void roValidateSizeOfGrfFile(const RoString& grfFileName)
    {
        const RoStreamSize fileSize = RoFileSystem::GetFileSize(grfFileName.asWStr());
        if (fileSize < RoGrfHeader::eHeaderMagicLength)
        {
            throw RoInvalidGrfException()
                << RoErrorInfoFileName(grfFileName)
                << RoErrorInfoDetail("File size is less than the header size!");
        }
    }

    roINLINE RoGrfHeader roGetHeaderOfGrfFile(const RoString& grfFileName)
    {
        std::fstream grfFile(
#if roPLATFORM_IS_WIN32
            grfFileName.asWStr_c_str(),
#else
            grfFileName.asUTF8_c_str(),
#endif // roPLATFORM_IS_WIN32
            std::ios::in|std::ios::binary);
        RoGrfHeader header;
        try
        {
            grfFile >> header;
        }
        catch (RoException& ex)
        {
            ex << RoErrorInfoFileName(grfFileName);
            throw;
        }
        return header;
    }

    roINLINE void roValidateHeaderVersionOfGrfFile(const RoGrfHeader& header, const RoString& grfFileName)
    {
        if (!RoGrfFactory::IsGrfVersionSupported(header.getVersion()))
        {
            throw RoInvalidGrfException()
                << RoErrorInfoDetail("Unsupported GRF version.")
                << RoErrorInfoFileName(grfFileName)
                << RoErrorInfoGrfVersion(header.getVersion());
        }
    }

    void roValidateHeaderOffsetOfGrfFile(const RoGrfHeader& header, const RoString& grfFileName)
    {
        if (RoFileSystem::GetFileSize(grfFileName.asWStr()) < header.getOffset())
        {
            throw RoInvalidGrfException()
            << RoErrorInfoFileName(grfFileName)
            << RoErrorInfoDetail("Invalid header offset.");
        }
    }
}

bool RoGrfFactory::IsGrfVersionSupported(uint32 version)
{
    uint32 majorVersion =  0xFF00 & version;
    switch (majorVersion)
    {
    // TODO: Uncomment these lines once GRF version 1 is implemented
    // case 0x0100:
    case 0x0200:
        return true;
        break;
    default:
        break;
    }
    return false;
}

RoGrfFactory::RoGrfFactory(RoGrfStorageBackendPtr store)
    : mStore(store)
{
}

RoGrfFactory::~RoGrfFactory()
{
}

RoGrfPtr RoGrfFactory::getGrf(const RoString& grfFileName)
{
    roValidateSizeOfGrfFile(grfFileName);

    RoGrfHeader header = roGetHeaderOfGrfFile(grfFileName);

    roValidateHeaderVersionOfGrfFile(header, grfFileName);

    roValidateHeaderOffsetOfGrfFile(header, grfFileName);

    return createGrfInstance(header, grfFileName);
}

RoGrfPtr RoGrfFactory::createGrfInstance(const RoGrfHeader& header, const RoString& grfFileName)
{
    RoGrfPtr grf;
    switch (header.getMajorVersion())
    {
    // TODO: Uncomment these lines once GRF version 1 is implemented
    //case 0x0100:
    //    grf.reset(new RoGrfVersion1(header, grfFileName));
    //    break;
    case 0x0200:
        {
            size_t grfId = mStore->getGrfId(grfFileName);
            grf.reset(new RoGrfVersion2(grfId, mStore, header, grfFileName));
        }
        break;
    default:
        break;
    }
    return grf;
}
