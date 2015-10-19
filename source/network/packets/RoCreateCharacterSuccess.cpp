#include "RoCreateCharacterSuccess.h"
#include "RoCharacterInformation.h"

roREGISTER_PACKET("CharacterCreateSuccess", RoCreateCharacterSuccess);

RoCreateCharacterSuccess::RoCreateCharacterSuccess()
{
    addStruct<RoCharacterInformation>("character", &RoCreateCharacterSuccess::mCharacter);
}

const RoCharacterInformation& RoCreateCharacterSuccess::getCharacter() const
{
    return mCharacter;
}
