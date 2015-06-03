////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_MESSAGE_H
#define ROKLEGEND_MESSAGE_H

#include "../RoPrerequisites.h"
#include "../RoSharedPtr.h"
#include "../task/RoTaskArgs.h"

class RoMessage;
roDEFINE_PTR(RoMessage);

/**
  * Brief description about RoMessage.
 **/
class RoMessage
{
public:
    explicit RoMessage(const RoString& function, const RoTaskArgs& args);
    virtual ~RoMessage();

    inline RoString getFunction() const
    {
        return mFunction;
    }

    inline RoTaskArgs& getArgs() const
    {
        return *mArgs;
    }

private:
    const RoString      mFunction;
    const RoTaskArgsPtr mArgs;
};

#endif // ROKLEGEND_MESSAGE_H
