#pragma once
#include "RoPacket.h"

enum class RoAccountGender
{
    UNKNOWN = -1,
    FEMALE,
    MALE,
    SERVER
};

class RoAuthenticatedAccount : public RoPacketT < RoAuthenticatedAccount >
{
public:
    RoAuthenticatedAccount();

    uint32 getId() const
    {
        return mId;
    }

    uint32 getLoginId1() const
    {
        return mLoginId1;
    }

    uint32 getLoginId2() const
    {
        return mLoginId2;
    }

    RoString getLastLoginIp() const
    {
        return mLastLoginIp;
    }

    RoString getLastLoginTime() const
    {
        return mLastLoginTime;
    }

    RoAccountGender getGender() const;
private:
    uint32 mId;
    uint32 mLoginId1;
    uint32 mLoginId2;
    RoString mLastLoginIp;
    RoString mLastLoginTime;
    uint8 mGender;
};
