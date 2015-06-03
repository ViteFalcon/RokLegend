////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoMessage.h"

RoMessage::RoMessage(const RoString& function, const RoTaskArgs& args)
    : mFunction(function)
    , mArgs(args.clone())
{
}

RoMessage::~RoMessage()
{
}
