////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_SERVER_H
#define ROKLEGEND_NET_SERVER_H

#include <core/RoPrerequisites.h>
#include <core/task/RoTaskArgs.h>

#include "RoNetServerType.h"
#include "RoNetServerStatus.h"

class RoNetServerActions
{
public:
    static const RoNetServerActions& Get(RoNetServerType serverType);

    RoNetServerActions(const RoNetServerType serverType)
        : mServerType(serverType)
    {
    }
    virtual ~RoNetServerActions()
    {
    }

    RoNetServerType getServerType() const
    {
        return mServerType;
    }

    virtual void onConnectionSuccess(const RoTaskArgs& args) const
    {
    }

    virtual void onConnectionFailure(const RoTaskArgs& args) const
    {
    }

    virtual void onDisconnection(const RoTaskArgs& args) const
    {
    }

private:
    const RoNetServerType mServerType;
};

#endif // ROKLEGEND_NET_SERVER_H
