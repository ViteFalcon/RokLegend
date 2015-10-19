#pragma once
#include "RoPacket.h"

roDEFINE_PACKET(RoDeleteCharacterCancelResponse)
{
public:
    enum class Code
    {
        SUCCESS,
        ERROR_UNKNOWN,
        ERROR_DATABASE
    };

    RoDeleteCharacterCancelResponse();
    ~RoDeleteCharacterCancelResponse() = default;

    uint32 getCharacterId() const;

    Code getResponseCode() const;

    RoString getResponseDescription() const;

private:
    uint32 mCharacterId;
    uint32 mResponseCode;
};