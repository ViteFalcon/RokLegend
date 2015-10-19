#include "RoDeleteCharacterAcceptRequest.h"
#include "RoPacketTranslator.h"

#include <core/RoStringUtil.h>

roREGISTER_PACKET("CharacterDeleteAcceptRequest", RoDeleteCharacterAcceptRequest);

RoDeleteCharacterAcceptRequest::RoDeleteCharacterAcceptRequest()
{
    add<uint32>("character_id", &RoDeleteCharacterAcceptRequest::mCharacterId);
    add<RoString>("birth_date", &RoDeleteCharacterAcceptRequest::mBirthDate);
}

void RoDeleteCharacterAcceptRequest::setCharacterId(const uint32 characterId)
{
    mCharacterId = characterId;
}

void RoDeleteCharacterAcceptRequest::setBirthDate(const RoDate& birthDate)
{
    uint16 year = uint16(birthDate.year()) % 100;
    uint16 month = birthDate.month().as_number();
    uint16 day = birthDate.day().as_number();
    mBirthDate = RoStringUtil::Format(L"%02d%02d%02d", year, month, day);
}
