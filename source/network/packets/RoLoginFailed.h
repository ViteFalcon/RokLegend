#pragma once
#include "RoPacket.h"

enum class RoLoginFailureReason
{
    UNKNOWN = -1,
    ACCOUNT_DOESNT_EXIST,
    INCORRECT_PASSWORD,
    REJECTED_FROM_SERVER,
    BLOCKED_ACCOUNT,
    NOT_LATEST_CLIENT,
    SERVER_BLOCKED_CONNECTION
};

roDEFINE_PACKET(RoLoginFailed)
{
public:
    RoLoginFailed();

    RoLoginFailureReason getReason() const;
private:
    uint8 mErrorCode;
};
