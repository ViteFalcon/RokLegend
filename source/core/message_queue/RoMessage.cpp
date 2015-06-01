////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnar�k Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoMessage.h"

RoMessage::RoMessage(const RoString& function, const RoPropertyMap& args)
    : mFunction(function)
    , mArgs(args)
{
}

RoMessage::~RoMessage()
{
}
