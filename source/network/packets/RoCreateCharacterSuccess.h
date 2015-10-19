#pragma once
#include "RoPacket.h"
#include "RoCharacterInformation.h"

class RoCreateCharacterSuccess : public RoPacketT<RoCreateCharacterSuccess>
{
public:
    RoCreateCharacterSuccess();
    ~RoCreateCharacterSuccess() = default;

    const RoCharacterInformation& getCharacter() const;
private:
    RoCharacterInformation mCharacter;
};