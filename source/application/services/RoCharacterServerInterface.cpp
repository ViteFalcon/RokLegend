#include "RoCharacterServerInterface.h"

#include <core/RoLog.h>
#include <core/RoStringUtil.h>

#include <network/events/RoPacketReceivedEvent.h>

#include <network/packets/RoCharacterListing.h>
#include <network/packets/RoCharacterInformation.h>
#include <network/packets/RoCreateCharacterFailed.h>
#include <network/packets/RoCreateCharacterRequest.h>
#include <network/packets/RoCreateCharacterSuccess.h>
#include <network/packets/RoGameLogin.h>
#include <network/packets/RoLoginSuccessful.h>

const RoString RoCharacterServerInterface::CHARACTER_SELECT_NOTIFICATION{ L"CharacterSelectNotification" };
const RoString RoCharacterServerInterface::CHARACTER_SELECT_PAGES{ L"CharacterSelectPage" };
const RoString RoCharacterServerInterface::BLOCKED_CHARACTER_INFORMATION{ L"BlockedCharacters" };
const RoString RoCharacterServerInterface::CHARACTER_LISTING_TASK{ L"CharactersReceived" };
const RoString RoCharacterServerInterface::LOGIN_FAILED_TASK{ L"GameLoginError" };
const RoString RoCharacterServerInterface::PINCODE_SYSTEM_DISABLED{ L"PinCodeSystemDisabled" };

namespace {
    static const RoString CREATE_CHARACTER_FAILED_NOTIFICATION{ L"CharacterCreateFailed" };
    static const RoString CREATE_CHARACTER_SUCCESS_NOTIFICATION{ L"CharacterCreateSuccess" };
}

RoCharacterServerInterface::RoCharacterServerInterface(RoLoginSuccessfulPtr accountInfo, RoCharacterListingPtr characterListing)
    : RoServerInterfaceT(RoNetServerType::CHARACTER)
    , mAccountInfo(accountInfo)
    , mCharacterListing(characterListing)
{
}

void RoCharacterServerInterface::loginToServerAtIndex(size_t serverIndex, const LoginCallback& loginCallback)
{
    RoCharacterServerCollection characterServers = mAccountInfo->getCharacterServers();
    if (characterServers.size() <= serverIndex)
    {
        roTHROW(RoInvalidOperation() << RoErrorInfoDetail("Server index is outside the bounds of number of servers"));
    }

    mLoginCallback.reset(loginCallback);

    auto server = characterServers.at(serverIndex);
    auto callback = std::bind(&RoCharacterServerInterface::connectResponse, this, std::placeholders::_1, std::placeholders::_2);
    connect(server->getIpAddress(), server->getPortNumber(), callback);
}

RoStringArray RoCharacterServerInterface::getServerNames() const
{
    RoCharacterServerCollection characterServers = mAccountInfo->getCharacterServers();
    if (characterServers.empty())
    {
        return RoStringArray{};
    }

    RoStringArray serverNames;
    serverNames.reserve(characterServers.size());
    for (auto server : characterServers)
    {
        RoString serverName = RoStringUtil::Format(L"%s (%d)", server->getName().asWStr_c_str(), server->getNumberOfPlayers());
        serverNames.push_back(serverName);
    }
    return serverNames;
}

void RoCharacterServerInterface::addTaskHandlers()
{
    addTaskHandler<RoPacketReceivedEvent>(CHARACTER_LISTING_TASK, &RoCharacterServerInterface::charactersReceived);
    addTaskHandler<RoPacketReceivedEvent>(CHARACTER_SELECT_NOTIFICATION, &RoCharacterServerInterface::loginSuccessful);
    addTaskHandler<RoPacketReceivedEvent>(CHARACTER_SELECT_PAGES, &RoCharacterServerInterface::characterSelectPages);
    addTaskHandler<RoPacketReceivedEvent>(BLOCKED_CHARACTER_INFORMATION, &RoCharacterServerInterface::blockedCharacters);
    addTaskHandler<RoPacketReceivedEvent>(LOGIN_FAILED_TASK, &RoCharacterServerInterface::loginFailed);
    addTaskHandler<RoPacketReceivedEvent>(PINCODE_SYSTEM_DISABLED, &RoCharacterServerInterface::pincodeSystem);
    addTaskHandler<RoPacketReceivedEvent>(CREATE_CHARACTER_SUCCESS_NOTIFICATION, &RoCharacterServerInterface::onSuccessfulCharacterCreation);
    addTaskHandler<RoPacketReceivedEvent>(CREATE_CHARACTER_FAILED_NOTIFICATION, &RoCharacterServerInterface::onFailedCharacterCreation);
}

void RoCharacterServerInterface::connectResponse(RoNetServerType type, RoOptionalString error)
{
    if (error)
    {
        invokeLoginCallback(std::make_shared<RoCharacterServerConnectFailed>(error.get()));
        return;
    }
    auto loginPacket = std::make_shared<RoGameLogin>();
    loginPacket->setAccountId(mAccountInfo->getId());
    loginPacket->setLoginId(mAccountInfo->getLoginId1());
    loginPacket->setLoginId2(mAccountInfo->getLoginId2());
    loginPacket->setGender(mAccountInfo->getGender());
    asyncSendPacket(loginPacket);
}

void RoCharacterServerInterface::invokeLoginCallback(RoCharacterServerLoginResultPtr result)
{
    if (!mLoginCallback.is_initialized())
    {
        roLOG_ERR << "Login callback is not initialized. Punting!";
        return;
    }
    auto& callback = mLoginCallback.get();
    callback(result);
}

void RoCharacterServerInterface::charactersReceived(const RoPacketReceivedEvent& args)
{
    auto characterListing = std::dynamic_pointer_cast<RoCharacterListing, RoPacket>(args.packet);
    mCharacterListing->fromProperties(characterListing->getProperties());
    invokeLoginCallback(std::make_shared<RoCharacterServerLoginSucceeded>(characterListing));
}

void RoCharacterServerInterface::loginSuccessful(const RoPacketReceivedEvent& args)
{
    roLOG_INFO << "Nothing to do after character-server login is successful";
}

void RoCharacterServerInterface::characterSelectPages(const RoPacketReceivedEvent& args)
{
    auto pageCount = args.getProperties().get("page_count").as<uint32>();
}

void RoCharacterServerInterface::blockedCharacters(const RoPacketReceivedEvent& args)
{
    roLOG_INFO << "Ignoring blocked characters for now.";
}

void RoCharacterServerInterface::loginFailed(const RoPacketReceivedEvent& args)
{
    invokeLoginCallback(std::make_shared<RoCharacterServerLoginFailed>());
}

namespace {
    RoPinCodeSystemRequest from_integer(uint16 value)
    {
        switch (value)
        {
        case 0:
            return RoPinCodeSystemRequest::DISABLED;
        case 1:
            return RoPinCodeSystemRequest::ENTER_PIN;
        case 2:
            return RoPinCodeSystemRequest::CREATE_NEW_PIN;
        case 3:
            return RoPinCodeSystemRequest::PIN_MUST_CHANGE;
        case 4:
            return RoPinCodeSystemRequest::CREATE_NEW_PIN2;
        case 5:
            return RoPinCodeSystemRequest::SHOW_MSG_STRING_1896;
        case 6:
            return RoPinCodeSystemRequest::UNABLE_TO_USE_KSSN_NUMBER;
        case 7:
            return RoPinCodeSystemRequest::SHOW_BUTTON_IN_CHAR_SELECT;
        case 8:
            return RoPinCodeSystemRequest::PINCODE_WAS_INCORRECT;
        default:
            break;
        }
        return RoPinCodeSystemRequest::UNKNOWN;
    }
}

void RoCharacterServerInterface::pincodeSystem(const RoPacketReceivedEvent& args)
{
    auto pincodeSystemState = args.getProperties().get(_H("state")).as<uint16>();
    auto pincodeRequest = from_integer(pincodeSystemState);
    invokeLoginCallback(std::make_shared<RoCharacterServerPinCodeSystem>(pincodeRequest));
}

void RoCharacterServerInterface::onSuccessfulCharacterCreation(const RoPacketReceivedEvent& args)
{
    auto response = std::dynamic_pointer_cast<RoCreateCharacterSuccess>(args.packet);
    roLOG_INFO << "RoCharacterServerInterface: Successfully created character. Name: " << response->getCharacter()->getName();
    if (!mCreateCharacterCallbacks || !mCreateCharacterCallbacks.get().successCallback)
    {
        roLOG_WARN << "RoCharacterServerInterface: No callbacks to inform successful character creation!";
        return;
    }
    mCreateCharacterCallbacks.get().successCallback(response->getCharacter());
    // Get rid of the current callback
    mCreateCharacterCallbacks.reset();
}

void RoCharacterServerInterface::onFailedCharacterCreation(const RoPacketReceivedEvent& args)
{
    auto response = std::dynamic_pointer_cast<RoCreateCharacterFailed>(args.packet);
    auto failureDescription = response->getFailureDescription();
    roLOG_INFO << "RoCharacterServerInterface: Failed to create character. Reason: " << failureDescription;
    if (!mCreateCharacterCallbacks || !mCreateCharacterCallbacks.get().failureCallback)
    {
        roLOG_WARN << "RoCharacterServerInterface: No callbacks to inform failed character creation!";
        return;
    }
    mCreateCharacterCallbacks.get().failureCallback(failureDescription);
    // Get rid of the current callback
    mCreateCharacterCallbacks.reset();
}

uint32 RoCharacterServerInterface::getCharacterPageCount() const
{
    return mCharacterPageCount.get_value_or(3);
}

RoCharacterListingPtr RoCharacterServerInterface::getCharacterListing() const
{
    return mCharacterListing;
}

void RoCharacterServerInterface::loginCharacterAtSlot(size_t slot)
{
    RoCharacterInformationPtr selectedCharacter;
    for each (auto character in mCharacterListing->getCharacters())
    {
        if (character->getSlot() == slot)
        {
            selectedCharacter = character;
            break;
        }
    }
    if (!selectedCharacter)
    {
        auto slotHint = RoStringUtil::Format(L"Slot: %d", slot);
        roTHROW(RoInvalidOperation()
            << RoErrorInfoDetail("The selected slot does not have a character associated with it.")
            << RoErrorInfoHint(slotHint));
    }
}

void RoCharacterServerInterface::createCharacter(const RoCreateCharacterCallbacks& callbacks, const RoString& name, uint16 hairColor, uint16 hairStyle)
{
    mCreateCharacterCallbacks = callbacks;
    auto request = std::make_shared<RoCreateCharacterRequest>();
    request->setName(name);
    request->setHairColor(hairColor);
    request->setHairStyle(hairStyle);
    asyncSendPacket(request);
}

RoCharacterServerConnectFailed::RoCharacterServerConnectFailed(const RoString& reason)
    : mReason(reason)
{
}

RoCharacterServerLoginResult::Type RoCharacterServerConnectFailed::getType() const
{
    return Type::CONNECT_FAILED;
}

RoString RoCharacterServerConnectFailed::getReason() const
{
    return mReason;
}

RoCharacterServerLoginResult::Type RoCharacterServerLoginFailed::getType() const
{
    return Type::LOGIN_FAILED;
}

RoString RoCharacterServerLoginFailed::getReason() const
{
    return RoString{ L"Rejected from server" };
}

RoCharacterServerLoginSucceeded::RoCharacterServerLoginSucceeded(RoCharacterListingPtr characterListing)
    : mCharacterListing(characterListing)
{
}

RoCharacterServerLoginResult::Type RoCharacterServerLoginSucceeded::getType() const
{
    return Type::SUCCESS;
}

RoCharacterListingPtr RoCharacterServerLoginSucceeded::getCharacterListing() const
{
    return mCharacterListing;
}

RoCharacterServerPinCodeSystem::RoCharacterServerPinCodeSystem(RoPinCodeSystemRequest request)
    : mRequest(request)
{
}

RoCharacterServerLoginResult::Type RoCharacterServerPinCodeSystem::getType() const
{
    return Type::PIN_CODE_SYSTEM;
}

RoPinCodeSystemRequest RoCharacterServerPinCodeSystem::getPinCodeRequest() const
{
    return mRequest;
}

RoString to_string(RoPinCodeSystemRequest request)
{
    switch (request)
    {
    case RoPinCodeSystemRequest::DISABLED:
        return L"Pincode System is disabled!";
    case RoPinCodeSystemRequest::ENTER_PIN:
        return L"Ask for pin";
    case RoPinCodeSystemRequest::CREATE_NEW_PIN:
        return L"Create new pin";
    case RoPinCodeSystemRequest::PIN_MUST_CHANGE:
        return L"Pin must be changed.";
    case RoPinCodeSystemRequest::CREATE_NEW_PIN2:
        return L"Create new pin2";
    case RoPinCodeSystemRequest::SHOW_MSG_STRING_1896:
        return L"Client shows message string (1896)";
    case RoPinCodeSystemRequest::UNABLE_TO_USE_KSSN_NUMBER:
        return L"Client shows a message (1897). Unable to use your KSSN number.";
    case RoPinCodeSystemRequest::SHOW_BUTTON_IN_CHAR_SELECT:
        return L"Char select shows a button.";
    case RoPinCodeSystemRequest::PINCODE_WAS_INCORRECT:
        return L"Pincode was incorrect!";
    default:
        break;
    }
    std::wstringstream unknownRequest;
    unknownRequest << L"Unknown pincode system state: " << as_integer(request);
    return unknownRequest.str();
}

std::ostream& operator<<(std::ostream& stream, const RoPinCodeSystemRequest& request)
{
    return stream << to_string(request);
}
