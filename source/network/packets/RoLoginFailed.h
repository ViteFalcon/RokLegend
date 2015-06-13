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

std::ostream& operator << (std::ostream& stream, const RoLoginFailureReason& reason);

roDEFINE_PACKET(RoLoginFailed)
{
public:
    RoLoginFailed();

    RoLoginFailureReason getReason() const;

    RoString getDetail() const
    {
        return mErrorDetail;
    }
private:
    uint8 mErrorCode;
    RoString mErrorDetail;
};
