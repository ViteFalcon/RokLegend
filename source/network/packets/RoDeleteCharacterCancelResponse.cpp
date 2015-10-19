#include "RoDeleteCharacterCancelResponse.h"

roREGISTER_PACKET("CharacterDeleteCancelResponse", RoDeleteCharacterCancelResponse);

RoDeleteCharacterCancelResponse::RoDeleteCharacterCancelResponse()
{
    add<uint32>("character_id", &RoDeleteCharacterCancelResponse::mCharacterId);
    add<uint32>("response_code", &RoDeleteCharacterCancelResponse::mResponseCode);
}

uint32 RoDeleteCharacterCancelResponse::getCharacterId() const
{
    return mCharacterId;
}

RoDeleteCharacterCancelResponse::Code RoDeleteCharacterCancelResponse::getResponseCode() const
{
    switch (mResponseCode)
    {
    case 1:
        return Code::SUCCESS;
    case 2:
        return Code::ERROR_DATABASE;
    default:
        break;
    }
    return Code::ERROR_UNKNOWN;
}

RoString RoDeleteCharacterCancelResponse::getResponseDescription() const
{
    switch (getResponseCode())
    {
    case Code::SUCCESS:
        return RoString{ L"Character deletion request was successfully canceled." };
    case Code::ERROR_DATABASE:
        return RoString{ L"There was a problem at the server-side that prevented canceling character deletion." };
    default:
        break;
    }
    return RoString{ L"There was an unknown error when trying to cancel the character deletion." };
}
