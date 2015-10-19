#pragma once
#include "RoPacket.h"

roDEFINE_PACKET(RoDeleteCharacterResponse)
{
public:
    enum class Code
    {
        SUCCESS,
        ERROR_UNKNOWN,
        ERROR_DATABASE,
        ERROR_CHARACTER_MUST_WITHDRAW_FROM_GUILD,
        ERROR_CHARACTER_MUST_WITHDRAW_FROM_PARTY
    };

    RoDeleteCharacterResponse();
    ~RoDeleteCharacterResponse() = default;

    uint32 getCharacterId() const;

    Code getResponseCode() const;

    RoString getResponseDescription() const;

    RoDate getDeleteDate() const;

private:
    uint32 mCharacterId;
    uint32 mResponseCode;
    uint32 mDeleteDate;
};