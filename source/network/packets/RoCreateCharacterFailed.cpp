#include "RoCreateCharacterFailed.h"

roREGISTER_PACKET("CharacterCreateFailed", RoCreateCharacterFailed);

RoCreateCharacterFailed::RoCreateCharacterFailed()
{
    add<uint8>("error_code", &RoCreateCharacterFailed::mErrorCode);
}

RoCreateCharacterFailure RoCreateCharacterFailed::getFailureCode() const
{
    switch (mErrorCode)
    {
    case 0x00:
        return RoCreateCharacterFailure::CHARACTER_NAME_ALREADY_EXISTS;
    case 0x01:
        return RoCreateCharacterFailure::NOT_ELIGIBLE_TO_CREATE_IN_SLOT;
    case 0x03:
        return RoCreateCharacterFailure::INVALID_SLOT;
    case 0xFF:
        return RoCreateCharacterFailure::INVALID_CHARACTER_NAME;
    default:
        break;
    }
    return RoCreateCharacterFailure::UNKNOWN;
}

RoString RoCreateCharacterFailed::getFailureDescription() const
{
    switch (getFailureCode())
    {
    case RoCreateCharacterFailure::CHARACTER_NAME_ALREADY_EXISTS:
        return RoString{ L"Character name already exists" };
    case RoCreateCharacterFailure::INVALID_CHARACTER_NAME:
        return RoString{ L"Invalid character name" };
    case RoCreateCharacterFailure::INVALID_SLOT:
        return RoString{ L"Invalid slot specified" };
    case RoCreateCharacterFailure::NOT_ELIGIBLE_TO_CREATE_IN_SLOT:
        return RoString{ L"You're not eligible to create character in this slot" };
    case RoCreateCharacterFailure::UNKNOWN:
    default:
        break;
    }
    return RoString{ L"Character creation failed due to unknown reason" };
}
