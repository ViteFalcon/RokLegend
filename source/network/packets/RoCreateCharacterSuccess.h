#pragma once
#include "RoPacket.h"

roFORWARD_DECL_PTR(RoCharacterInformation);

roDEFINE_PACKET(RoCreateCharacterSuccess)
{
public:
    RoCreateCharacterSuccess();
    ~RoCreateCharacterSuccess() = default;

    RoCharacterInformationPtr getCharacter() const
    {
        return mCharacter;
    }
private:
    RoCharacterInformationPtr mCharacter;
};