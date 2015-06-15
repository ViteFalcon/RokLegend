#pragma once
#include "RoPacket.h"
#include "RoCharacterServer.h"
#include "RoAccountGender.h"

using RoCharacterServerCollection = RoVector < RoCharacterServerPtr > ;

roDEFINE_PACKET(RoLoginSuccessful)
{
public:
    RoLoginSuccessful();

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

    RoCharacterServerCollection getCharacterServers() const
    {
        return mCharacterServers;
    }
private:
    uint32 mId;
    uint32 mLoginId1;
    uint32 mLoginId2;
    RoString mLastLoginIp;
    RoString mLastLoginTime;
    uint8 mGender;
    RoCharacterServerCollection mCharacterServers;
};
