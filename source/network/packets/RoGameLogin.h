#pragma once
#include "RoPacket.h"
#include "RoAccountGender.h"

roDEFINE_PACKET(RoGameLogin)
{
public:
    RoGameLogin();

    uint32 getAccountId() const
    {
        return mAccountId;
    }

    void setAccountId(uint32 val)
    {
        mAccountId = val;
    }

    uint32 getLoginId() const
    {
        return mLoginId;
    }

    void setLoginId(uint32 val)
    {
        mLoginId = val;
    }

    uint32 getLoginId2() const
    {
        return mLoginId2;
    }

    void setLoginId2(uint32 val)
    {
        mLoginId2 = val;
    }

    RoAccountGender getGender() const;
    void setGender(RoAccountGender val);
private:
    uint32 mAccountId;
    uint32 mLoginId;
    uint32 mLoginId2;
    uint8 mGender;
};
