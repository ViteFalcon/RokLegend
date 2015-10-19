#pragma once
#include "RoPacket.h"

roDEFINE_PACKET(RoDeleteCharacterCancelRequest)
{
public:
    RoDeleteCharacterCancelRequest();
    ~RoDeleteCharacterCancelRequest() = default;

    void setCharacterId(const uint32 characterId);
private:
    uint32 mCharacterId;
};