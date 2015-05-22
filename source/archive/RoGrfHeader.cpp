////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoGrfHeader.h"

#include <core/RoException.h>
#include <core/RoErrorInfo.h>

#include <sstream>
//==============================================================================
const std::string   RoGrfHeader::sHeaderMagic       = "Master of Magic";
const uint8         RoGrfHeader::sFileStatInfoSize  = 17;
const uint8         RoGrfHeader::sKeyLength         = 14;
const uint8         RoGrfHeader::sHeaderLength      = 46;
//------------------------------------------------------------------------------
const uint8     RoGrfHeader::sGrfIsEncodedKey[RoGrfHeader::eGrfKeyLength] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E
};
//------------------------------------------------------------------------------
const uint8     RoGrfHeader::sGrfNotEncodedKey[RoGrfHeader::eGrfKeyLength] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//==============================================================================
RoGrfHeader& RoGrfHeader::fromStream(std::istream& stream)
{
    stream.read(reinterpret_cast<char*>(this), RoGrfHeader::sHeaderLength);
    // Check if the header magic is valid
    if (!isMagicValid())
    {
        roTHROW(RoInvalidGrfException()
            << RoErrorInfoDetail("Invalid GRF header-magic."));
    }
    // Check if the header 'key' is valid
    if (!isKeyValid())
    {
        roTHROW(RoInvalidGrfException()
            << RoErrorInfoDetail("Invalid header-key."));
    }
    // If all is ok, add the header length to the offset
    offset += RoGrfHeader::sHeaderLength;
    return *this;
}
//------------------------------------------------------------------------------
bool RoGrfHeader::isMagicValid() const
{
    return memcmp(magic, sHeaderMagic.c_str(), sHeaderMagic.length()) == 0;
}
//------------------------------------------------------------------------------
bool RoGrfHeader::isKeyValid() const
{
    return
        memcmp(key, sGrfIsEncodedKey, eGrfKeyLength) == 0 ||
        memcmp(key, sGrfNotEncodedKey, eGrfKeyLength) == 0;
}
//------------------------------------------------------------------------------
RoString RoGrfHeader::getVersionString() const
{
    std::stringstream versionStr;
    versionStr << (getMajorVersion() >> 8) << "." << getMinorVersion();
    return versionStr.str();
}
//------------------------------------------------------------------------------
bool RoGrfHeader::isKeyEncoded() const
{
    return memcmp(key, sGrfIsEncodedKey, eGrfKeyLength) == 0;
}
//==============================================================================
