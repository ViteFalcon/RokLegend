#include "RoCreateCharacterRequest.h"

roREGISTER_PACKET("CharacterCreate", RoCreateCharacterRequest);

RoCreateCharacterRequest::RoCreateCharacterRequest()
{
    add<RoString>("name", &RoCreateCharacterRequest::mName);
    add<uint8>("slot", &RoCreateCharacterRequest::mSlot);
    add<uint16>("hair_color", &RoCreateCharacterRequest::mHairColor);
    add<uint16>("hair", &RoCreateCharacterRequest::mHairStyle);
}
