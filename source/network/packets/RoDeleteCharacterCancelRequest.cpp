#include "RoDeleteCharacterCancelRequest.h"
#include "RoPacketTranslator.h"

roREGISTER_PACKET("CharacterDeleteCancelRequest", RoDeleteCharacterCancelRequest);

RoDeleteCharacterCancelRequest::RoDeleteCharacterCancelRequest()
{
    add<uint32>("character_id", &RoDeleteCharacterCancelRequest::mCharacterId);
}

void RoDeleteCharacterCancelRequest::setCharacterId(const uint32 characterId)
{
    mCharacterId = characterId;
}
