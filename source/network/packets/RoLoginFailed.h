#include "RoLoginFailed.h"

RoLoginFailed::RoLoginFailed()
    : mErrorCode(-1)
{
    add<uint8>("error_code", &RoLoginFailed::mErrorCode);
}

RoLoginFailureReason RoLoginFailed::getReason() const
{
    switch (mErrorCode)
    {
    case 0:
        return RoLoginFailureReason::ACCOUNT_DOESNT_EXIST;
    case 1:
        return RoLoginFailureReason::INCORRECT_PASSWORD;
    case 3:
        return RoLoginFailureReason::REJECTED_FROM_SERVER;
    case 4:
        return RoLoginFailureReason::BLOCKED_ACCOUNT;
    case 5:
        return RoLoginFailureReason::NOT_LATEST_CLIENT;
    case 6:
        return RoLoginFailureReason::SERVER_BLOCKED_CONNECTION;
    default:
        break;
    }
    return RoLoginFailureReason::UNKNOWN;
}
