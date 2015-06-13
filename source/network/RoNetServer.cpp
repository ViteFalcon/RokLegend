////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////


#include "RoNetServer.h"
#include <core/task/RoTaskManager.h>
#include <core/message_queue/RoMessageQueue.h>

#include <tbb/concurrent_hash_map.h>

#include "events/RoServerConnectedEvent.h"
#include "events/RoServerConnectionFailedEvent.h"
#include "events/RoServerDisconnectedEvent.h"

class RoNetLoginServerActions : public RoNetServerActions
{
public:
    RoNetLoginServerActions()
        : RoNetServerActions(RoNetServerType::LOGIN)
    {
    }

    virtual void onConnectionSuccess(const RoServerConnectedEvent& args) const
    {
        roPOST_MSG(LoginServerConnected, args);
    }

    virtual void onConnectionFailure(const RoServerConnectRequestFailedEvent& args) const
    {
        roPOST_MSG(LoginServerConnectionFailed, args);
    }

    virtual void onDisconnection(const RoServerDisconnectedEvent& args) const override
    {
        roPOST_MSG(LoginServerDisconnected, args);
    }
};

class RoNetCharacterServerActions : public RoNetServerActions
{
public:
    RoNetCharacterServerActions()
        : RoNetServerActions(RoNetServerType::LOGIN)
    {
    }

    virtual void onConnectionSuccess(const RoServerConnectedEvent& args) const
    {
        roPOST_MSG(CharacterServerConnected, args);
    }

    virtual void onConnectionFailure(const RoServerConnectRequestFailedEvent& args) const
    {
        roPOST_MSG(CharacterServerConnectionFailed, args);
    }

    virtual void onDisconnection(const RoServerDisconnectedEvent& args) const override
    {
        roPOST_MSG(CharacterServerDisconnected, args);
    }
};

class RoNetMapServerActions : public RoNetServerActions
{
public:
    RoNetMapServerActions()
        : RoNetServerActions(RoNetServerType::LOGIN)
    {
    }

    virtual void onConnectionSuccess(const RoServerConnectedEvent& args) const
    {
        roPOST_MSG(MapServerConnected, args);
    }

    virtual void onConnectionFailure(const RoServerConnectRequestFailedEvent& args) const
    {
        roPOST_MSG(MapServerConnectionFailed, args);
    }

    virtual void onDisconnection(const RoServerDisconnectedEvent& args) const override
    {
        roPOST_MSG(MapServerDisconnected, args);
    }
};

const RoNetServerActions& RoNetServerActions::Get(RoNetServerType serverType)
{
    static const RoNetLoginServerActions loginServerActions;
    static const RoNetCharacterServerActions characterServerActions;
    static const RoNetMapServerActions mapServerActions;
    static const RoNetServerActions defaultServerActions(RoNetServerType::NONE);

    switch(serverType)
    {
    case RoNetServerType::LOGIN:
        return loginServerActions;
    case RoNetServerType::CHARACTER:
        return characterServerActions;
    case RoNetServerType::MAP:
        return mapServerActions;
    }

    return defaultServerActions;
}

