#pragma once
#include "RoPacket.h"

roDEFINE_PACKET(RoDeleteCharacterRequest)
{
public:
    RoDeleteCharacterRequest();
    ~RoDeleteCharacterRequest() = default;

    void setCharacterId(const uint32 characterId);
private:
    uint32 mCharacterId;
};
