#pragma once
#include "RoPacket.h"

roDEFINE_PACKET(RoLoginCredentials)
{
public:
    RoLoginCredentials();

    void setUsername(const RoString& username);
    void setPassword(const RoString& password);
private:
    RoString mUsername;
    RoString mPassword;
};
