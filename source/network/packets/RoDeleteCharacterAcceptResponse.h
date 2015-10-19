#pragma once
#include "RoPacket.h"

roDEFINE_PACKET(RoDeleteCharacterAcceptResponse)
{
public:
    enum class Code
    {
        SUCCESS,
        ERROR_UNKNOWN,
        ERROR_DELETION_NOT_ALLOWED_BY_SERVER,
        ERROR_DATABASE,
        ERROR_DELETION_NOT_POSSIBLE_AT_THIS_TIME,
        ERROR_DOB_DOES_NOT_MATCH
    };
    RoDeleteCharacterAcceptResponse();
    ~RoDeleteCharacterAcceptResponse() = default;

    uint32 getCharacterId() const;

    Code getResponseCode() const;

    RoString getResponseDescription() const;
private:
    uint32 mCharacterId;
    uint32 mResponse;
};