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
}

std::string to_string(const RoAccountGender gender)
{
    switch (gender)
    {
    case RoAccountGender::FEMALE:
        return "Female";
    case RoAccountGender::MALE:
        return "Male";
    case RoAccountGender::SERVER:
        return "Server";
    default:
        break;
    }
    return "Unknown Gender";
}
