#include "RoAuthenticatedAccount.h"
#include "RoPacketTranslator.h"

/*roREGISTER_PACKET("AccountServerInfo", RoAuthenticatedAccount);

RoAuthenticatedAccount::RoAuthenticatedAccount()
{
    add<uint32>("id", &RoAuthenticatedAccount::mId);
    add<uint32>("login_id1", &RoAuthenticatedAccount::mLoginId1);
    add<uint32>("login_id2", &RoAuthenticatedAccount::mLoginId2);
    add<RoString>("last_login_ip", &RoAuthenticatedAccount::mLastLoginIp);
    add<RoString>("last_login_time", &RoAuthenticatedAccount::mLastLoginTime);
    add<uint8>("account_gender", &RoAuthenticatedAccount::mGender);
}

RoAccountGender RoAuthenticatedAccount::getGender() const
{
    switch (mGender)
    {
    case 0:
        return RoAccountGender::FEMALE;
    case 1:
        return RoAccountGender::MALE;
    case 2:
        return RoAccountGender::SERVER;
    default:
        break;
    }
    return RoAccountGender::UNKNOWN;
}*/
