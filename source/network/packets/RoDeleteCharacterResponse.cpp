#include "RoDeleteCharacterResponse.h"
#include "RoPacketTranslator.h"

roREGISTER_PACKET("CharacterDeleteResponse", RoDeleteCharacterResponse);

RoDeleteCharacterResponse::RoDeleteCharacterResponse()
{
    add<uint32>("character_id", &RoDeleteCharacterResponse::mCharacterId);
    add<uint32>("response_code", &RoDeleteCharacterResponse::mResponseCode);
    add<uint32>("delete_date", &RoDeleteCharacterResponse::mDeleteDate);
}

uint32 RoDeleteCharacterResponse::getCharacterId() const
{
    return mCharacterId;
}

RoDeleteCharacterResponse::Code RoDeleteCharacterResponse::getResponseCode() const
{
    switch (mResponseCode)
    {
    case 1:
        return Code::SUCCESS;
    case 3:
        return Code::ERROR_DATABASE;
    case 4:
        return Code::ERROR_CHARACTER_MUST_WITHDRAW_FROM_GUILD;
    case 5:
        return Code::ERROR_CHARACTER_MUST_WITHDRAW_FROM_PARTY;
    }
    return Code::ERROR_UNKNOWN;
}

RoDate RoDeleteCharacterResponse::getDeleteDate() const
{
    return RoDate(mDeleteDate);
}

RoString RoDeleteCharacterResponse::getResponseDescription() const
{
    switch (getResponseCode())
    {
    case Code::SUCCESS:
        return RoString{ L"Character deletion request accepted." };
    case Code::ERROR_DATABASE:
        return RoString{ L"Server had a database issue when character deletion was requested." };
    case Code::ERROR_CHARACTER_MUST_WITHDRAW_FROM_GUILD:
        return RoString{ L"To delete a character you must withdraw it from the guild." };
    case Code::ERROR_CHARACTER_MUST_WITHDRAW_FROM_PARTY:
        return RoString{ L"To delete a character you must withdraw it from the party." };
    default:
        break;
    }
    return RoString{ L"An unexpected server error occurred when attempting to request character deletion." };
}
