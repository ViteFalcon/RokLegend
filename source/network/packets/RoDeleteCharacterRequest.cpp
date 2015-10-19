#include "RoDeleteCharacterRequest.h"
#include "RoPacketTranslator.h"

roREGISTER_PACKET("CharacterDeleteRequest", RoDeleteCharacterRequest);

RoDeleteCharacterRequest::RoDeleteCharacterRequest()
{
    add<uint32>("character_id", &RoDeleteCharacterRequest::mCharacterId);
}

void RoDeleteCharacterRequest::setCharacterId(const uint32 characterId)
{
    mCharacterId = characterId;
}
