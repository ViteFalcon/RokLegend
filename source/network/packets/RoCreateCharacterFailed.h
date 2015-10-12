#pragma once
#include "RoPacket.h"

enum class RoCreateCharacterFailure
{
    UNKNOWN,
    CHARACTER_NAME_ALREADY_EXISTS,
    INVALID_CHARACTER_NAME,
    NOT_ELIGIBLE_TO_CREATE_IN_SLOT
};

roDEFINE_PACKET(RoCreateCharacterFailed)
{
public:
    RoCreateCharacterFailed();
    ~RoCreateCharacterFailed() = default;

    RoCreateCharacterFailure getFailureCode() const;

    RoString getFailureDescription() const;
private:
    uint8 mErrorCode;
};