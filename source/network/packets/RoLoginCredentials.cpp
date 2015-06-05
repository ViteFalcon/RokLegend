#include "RoLoginCredentials.h"

roREGISTER_PACKET("MasterLogin", RoLoginCredentials);

RoLoginCredentials::RoLoginCredentials()
{
    add<RoString>("username", &RoLoginCredentials::mUsername);
    add<RoString>("password", &RoLoginCredentials::mPassword);
}

void RoLoginCredentials::setUsername(const RoString& username)
{
    mUsername = username;
}

void RoLoginCredentials::setPassword(const RoString& password)
{
    mPassword = password;
}
