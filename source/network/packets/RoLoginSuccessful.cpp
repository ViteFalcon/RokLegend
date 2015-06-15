#include "RoLoginSuccessful.h"

roREGISTER_PACKET("AccountServerInfo", RoLoginSuccessful);

RoLoginSuccessful::RoLoginSuccessful()
{
    add<uint32>("account_id", &RoLoginSuccessful::mId);
    add<uint32>("login_id1", &RoLoginSuccessful::mLoginId1);
    add<uint32>("login_id2", &RoLoginSuccessful::mLoginId2);
    add<RoString>("last_login_ip", &RoLoginSuccessful::mLastLoginIp);
    add<RoString>("last_login_time", &RoLoginSuccessful::mLastLoginTime);
    add<uint8>("account_gender", &RoLoginSuccessful::mGender);
    addList<RoCharacterServer>("servers", &RoLoginSuccessful::mCharacterServers);
}

RoAccountGender RoLoginSuccessful::getGender() const
{
    return roGetAccountGender(mGender);
}
