#pragma once
#include "RoPacket.h"

class RoLoginCredentials : public RoPacketT < RoLoginCredentials >
{
public:
    RoLoginCredentials();

    void setUsername(const RoString& username);
    void setPassword(const RoString& password);
private:
    RoString mUsername;
    RoString mPassword;
};
