#pragma once
#include "RoPacket.h"

roDEFINE_PACKET(RoDeleteCharacterAcceptRequest)
{
public:
    RoDeleteCharacterAcceptRequest();
    ~RoDeleteCharacterAcceptRequest() = default;

    void setCharacterId(const uint32 characterId);
    void setBirthDate(const RoDate& birthDate);
private:
    uint32 mCharacterId;
    RoString mBirthDate;
};
