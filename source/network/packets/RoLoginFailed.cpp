#include "RoLoginFailed.h"

roREGISTER_PACKET("MasterLoginError", RoLoginFailed);

RoLoginFailed::RoLoginFailed()
    : mErrorCode(-1)
{
    add<uint8>("error_code", &RoLoginFailed::mErrorCode);
    add<RoString>("error_detail", &RoLoginFailed::mErrorDetail);
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

std::ostream& operator<<(std::ostream& stream, const RoLoginFailureReason& reason)
{
    switch (reason)
    {
    case RoLoginFailureReason::ACCOUNT_DOESNT_EXIST:
        stream << "Account does not exist";
        break;
    case RoLoginFailureReason::INCORRECT_PASSWORD:
        stream << "Incorrect password";
        break;
    case RoLoginFailureReason::REJECTED_FROM_SERVER:
        stream << "Rejected from server";
        break;
    case RoLoginFailureReason::BLOCKED_ACCOUNT:
        stream << "Account is currently banned by the server";
        break;
    case RoLoginFailureReason::NOT_LATEST_CLIENT:
        stream << "Client is out-of-date";
        break;
    case RoLoginFailureReason::SERVER_BLOCKED_CONNECTION:
        stream << "Server blocked connection";
        break;
    default:
        stream << "UNKNOWN login failure reason";
        break;
    }
    return stream;
}
