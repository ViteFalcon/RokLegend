#pragma once
#include "RoServerInterface.h"

struct RoPacketReceivedEvent;
roFORWARD_DECL_PTR(RoCharacterListing);
roFORWARD_DECL_PTR(RoLoginSuccessful);

enum class RoPinCodeSystemRequest
{
    UNKNOWN,
    DISABLED,
    ENTER_PIN,
    CREATE_NEW_PIN,
    PIN_MUST_CHANGE,
    CREATE_NEW_PIN2,
    SHOW_MSG_STRING_1896,       // ???
    UNABLE_TO_USE_KSSN_NUMBER,  // ???
    SHOW_BUTTON_IN_CHAR_SELECT, // ???
    PINCODE_WAS_INCORRECT
};

RoString to_string(RoPinCodeSystemRequest request);
std::ostream& operator << (std::ostream& stream, const RoPinCodeSystemRequest& request);

roFORWARD_DECL_PTR(RoCharacterServerLoginResult);

class RoCharacterServerInterface : public RoServerInterfaceT < RoCharacterServerInterface >
{
public:
    using LoginCallback = std::function < void(RoCharacterServerLoginResultPtr) > ;

    RoCharacterServerInterface(RoLoginSuccessfulPtr accountInfo, RoCharacterListingPtr characterListing);
    ~RoCharacterServerInterface() = default;

    void loginToServerAtIndex(size_t serverIndex, const LoginCallback& loginCallback);

    RoStringArray getServerNames() const;

    uint32 getCharacterPageCount() const;

    RoCharacterListingPtr getCharacterListing() const;

protected:
    virtual void addTaskHandlers() override;

private:
    void connectResponse(RoNetServerType type, RoOptionalString error);
    void invokeLoginCallback(RoCharacterServerLoginResultPtr result);
    void charactersReceived(const RoPacketReceivedEvent& args);
    void loginSuccessful(const RoPacketReceivedEvent& args);
    void characterSelectPages(const RoPacketReceivedEvent& args);
    void blockedCharacters(const RoPacketReceivedEvent& args);
    void loginFailed(const RoPacketReceivedEvent& args);
    void pincodeSystem(const RoPacketReceivedEvent& args);

private: // static
    static const RoString CHARACTER_SELECT_NOTIFICATION;
    static const RoString CHARACTER_SELECT_PAGES;
    static const RoString BLOCKED_CHARACTER_INFORMATION;
    static const RoString CHARACTER_LISTING_TASK;
    static const RoString LOGIN_FAILED_TASK;
    static const RoString PINCODE_SYSTEM_DISABLED;

private:
    RoLoginSuccessfulPtr mAccountInfo;
    RoCharacterListingPtr mCharacterListing;
    optional<LoginCallback> mLoginCallback;
    RoOptionalUInt32 mCharacterPageCount;
};

class RoCharacterServerLoginResult
{
public:
    enum class Type
    {
        CONNECT_FAILED,
        LOGIN_FAILED,
        SUCCESS,
        PIN_CODE_SYSTEM
    };

    RoCharacterServerLoginResult() = default;
    virtual ~RoCharacterServerLoginResult() = default;

    virtual Type getType() const = 0;
};

class RoCharacterServerConnectFailed : public RoCharacterServerLoginResult
{
public:
    RoCharacterServerConnectFailed(const RoString& reason);

    virtual Type getType() const override;

    RoString getReason() const;

private:
    const RoString mReason;
};

class RoCharacterServerLoginFailed : public RoCharacterServerLoginResult
{
public:
    virtual Type getType() const override;

    RoString getReason() const;
};

class RoCharacterServerLoginSucceeded : public RoCharacterServerLoginResult
{
public:
    RoCharacterServerLoginSucceeded(RoCharacterListingPtr characterListing);

    virtual Type getType() const override;

    RoCharacterListingPtr getCharacterListing() const;
private:
    RoCharacterListingPtr mCharacterListing;
};

class RoCharacterServerPinCodeSystem : public RoCharacterServerLoginResult
{
public:
    RoCharacterServerPinCodeSystem(RoPinCodeSystemRequest request);

    virtual Type getType() const override;

    RoPinCodeSystemRequest getPinCodeRequest() const;
private:
    const RoPinCodeSystemRequest mRequest;
};
