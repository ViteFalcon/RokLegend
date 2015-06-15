#include "RoGameLogin.h"

roREGISTER_PACKET("GameLogin", RoGameLogin);

RoGameLogin::RoGameLogin()
{
    add<uint32>("account_id", &RoGameLogin::mAccountId);
    add<uint32>("login_id", &RoGameLogin::mLoginId);
    add<uint32>("login_id2", &RoGameLogin::mLoginId2);
    add<uint8>("account_sex", &RoGameLogin::mGender);
}

RoAccountGender RoGameLogin::getGender() const
{
    return roGetAccountGender(mGender);
}

void RoGameLogin::setGender(RoAccountGender gender)
{
    mGender = (uint8) as_integer(gender);
}
