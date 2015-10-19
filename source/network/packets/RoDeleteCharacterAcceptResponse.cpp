#include "RoDeleteCharacterAcceptResponse.h"
#include "RoPacketTranslator.h"

roREGISTER_PACKET("CharacterDeleteAcceptResponse", RoDeleteCharacterAcceptResponse);

RoDeleteCharacterAcceptResponse::RoDeleteCharacterAcceptResponse()
{
    add<uint32>("character_id", &RoDeleteCharacterAcceptResponse::mCharacterId);
    add<uint32>("response_code", &RoDeleteCharacterAcceptResponse::mResponse);
}

uint32 RoDeleteCharacterAcceptResponse::getCharacterId() const
{
    return mCharacterId;
}

RoDeleteCharacterAcceptResponse::Code RoDeleteCharacterAcceptResponse::getResponseCode() const
{
    switch (mResponse)
    {
    case 1:
        return Code::SUCCESS;
    case 2:
        return Code::ERROR_DELETION_NOT_ALLOWED_BY_SERVER;
    case 3:
        return Code::ERROR_DATABASE;
    case 4:
        return Code::ERROR_DELETION_NOT_POSSIBLE_AT_THIS_TIME;
    case 5:
        return Code::ERROR_DOB_DOES_NOT_MATCH;
    default:
        break;
    }
    return Code::ERROR_UNKNOWN;
}

RoString RoDeleteCharacterAcceptResponse::getResponseDescription() const
{
    switch (getResponseCode())
    {
    case Code::SUCCESS:
        return RoString{ L"Character was deleted successfully." };
    case Code::ERROR_DATABASE:
        return RoString{ L"There was an error in the server-side that caused character deletion to fail." };
    case Code::ERROR_DELETION_NOT_ALLOWED_BY_SERVER:
        return RoString{ L"Character deletion is not allowed by the server." };
    case Code::ERROR_DELETION_NOT_POSSIBLE_AT_THIS_TIME:
        return RoString{ L"Character deletion is not allowed at this time." };
    case Code::ERROR_DOB_DOES_NOT_MATCH:
        return RoString{ L"Date of birth entered doesn't match what the server has in record." };
    default:
        break;
    }
    return RoString{ L"An unknown error occurred when confirming character deletion." };
}
