////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////


#include "RoNetworkManager.h"
#include "RoNetPacketDb.h"
#include "RoNetServer.h"
#include "RoNetTcpConnection.h"

#include <core/RoDict.h>

#include "events/RoServerConnectRequestEvent.h"
#include "events/RoServerConnectRequestFailedEvent.h"
#include "events/RoServerDisconnectRequestEvent.h"
#include "events/RoServerConnectedEvent.h"
#include "events/RoServerDisconnectedEvent.h"
#include "events/RoSendPacketToServerEvent.h"
#include "events/RoPacketReceivedEvent.h"

void RoNetworkManager::Connect(RoNetServerType serverType, const RoString& ipAddress, const RoString& portNumber)
{
    RoServerConnectRequestEvent event;
    event.serverType = serverType;
    event.ipAddress = ipAddress;
    event.portNumber = portNumber;
    roPOST_MSG(NetworkServerConnect, event);
}

void RoNetworkManager::SendToServer(RoNetServerType serverType, RoPacketPtr packet)
{
    RoSendPacketToServerEvent event;
    event.packet = packet;
    switch (serverType)
    {
    case RoNetServerType::LOGIN:
        roPOST_MSG(NetworkLoginServerSend, event);
        break;
    case RoNetServerType::CHARACTER:
        roPOST_MSG(NetworkCharacterServerSend, event);
        break;
    case RoNetServerType::MAP:
        roPOST_MSG(NetworkMapServerSend, event);
        break;
    default:
        roLOG_ERR << "Cannot send packet to unrecognized server-type: " << as_integer(serverType);
        break;
    }
}

void RoNetworkManager::Disconnect(RoNetServerType serverType)
{
    RoServerDisconnectRequestEvent event;
    event.serverType = serverType;
    roPOST_MSG(NetworkLoginServerDisconnect, event);
}

RoNetworkManager::RoNetworkManager(const RoString &packetDb)
    : mMessages(RoMessageQueue::Get(RoMessageQueue::eMessageQueue_Network))
    , mPacketDb(RoNetPacketDb::LoadFromSource(packetDb))
{
    roBOUND_TASK(_NetworkUpdate, &RoNetworkManager::update);
    roBOUND_TASK(NetworkServerConnect, &RoNetworkManager::connect);
    roBOUND_TASK(NetworkServerDisconnect, &RoNetworkManager::disconnect);
    roBOUND_TASK(NetworkServerConnected, &RoNetworkManager::serverConnected);
    roBOUND_TASK(NetworkServerConnectFailed, &RoNetworkManager::serverConnectionFailed);
    roBOUND_TASK(NetworkServerDisconnected, &RoNetworkManager::serverDisconnected);
    roBOUND_TASK(NetworkLoginServerSend, &RoNetworkManager::sendToLoginServer);
    roBOUND_TASK(NetworkCharacterServerSend, &RoNetworkManager::sendToCharacterServer);
    roBOUND_TASK(NetworkMapServerSend, &RoNetworkManager::sendToMapServer);

    mOkToUpdate.store(true);
}

RoNetworkManager::~RoNetworkManager()
{
    for (int i = 0; i < (int) RoNetServerType::COUNT; ++i)
    {
        mConnections[i].reset();
    }
}

void RoNetworkManager::sendToLoginServer(const RoTaskArgs& args)
{
    sendToServer(RoNetServerType::LOGIN, "Login", args);
}

void RoNetworkManager::sendToCharacterServer(const RoTaskArgs& args)
{
    sendToServer(RoNetServerType::CHARACTER, "Character", args);
}

void RoNetworkManager::sendToMapServer(const RoTaskArgs& args)
{
    sendToServer(RoNetServerType::MAP, "Map", args);
}

void RoNetworkManager::sendToServer(const RoNetServerType serverType, const char* name, const RoTaskArgs& args)
{
    RoNetTcpConnectionPtr& serverConnection = mConnections[as_integer(serverType)];
    if (!serverConnection)
    {
        roLOG_ERR << name << " server is not connected for this operation to be completed.";
        return;
    }
    auto event = as_event_args<RoSendPacketToServerEvent>(args);
    RoNetPacketPtr packet = mPacketDb->getPacketByAction(event.packet->getActionName());
    packet->fromProperties(event.packet->getProperties());
    serverConnection->send(packet);
}

void RoNetworkManager::serverConnected(const RoTaskArgs& args)
{
    auto event = as_event_args<RoServerConnectedEvent>(args);
    RoNetServerActions::Get(event.serverType).onConnectionSuccess(args);
}

void RoNetworkManager::serverConnectionFailed(const RoTaskArgs& args)
{
    roLOG_WARN << "Seems like a network connection failed :(";
    auto event = as_event_args<RoServerConnectRequestFailedEvent>(args);
    RoNetServerActions::Get(event.serverType).onConnectionFailure(args);
}

void RoNetworkManager::serverDisconnected(const RoTaskArgs& args)
{
    auto event = as_event_args<RoServerDisconnectedEvent>(args);
    mConnections[as_integer(event.serverType)].reset();
    RoNetServerActions::Get(event.serverType).onDisconnection(args);
}

void RoNetworkManager::connect(const RoTaskArgs& args)
{
    auto event = as_event_args<RoServerConnectRequestEvent>(args);
    connectToServer(event.serverType, event.ipAddress, event.portNumber);
}

void RoNetworkManager::disconnect(const RoTaskArgs& args)
{
    auto event = as_event_args<RoServerDisconnectRequestEvent>(args);
    disconnectFrom(event.serverType);
}

void RoNetworkManager::update(const RoTaskArgs &)
{
    bool expected = true;
    if (mOkToUpdate.compare_exchange_strong(expected, false))
    {
        mMessages.dispatch();
        mOkToUpdate.store(true);
    }
}

void RoNetworkManager::connectToServer(RoNetServerType serverType, const RoString &host, const RoString &port)
{
    assert(RoNetServerType::NONE != serverType && RoNetServerType::COUNT != serverType);
    mConnections[as_integer(serverType)].reset();
    RoNetTcpConnectionPtr& serverConnection = mConnections[as_integer(serverType)];
    serverConnection.reset(new RoNetTcpConnection(
        *this,
        *mPacketDb,
        serverType,
        host,
        port));
}

void RoNetworkManager::disconnectFrom(RoNetServerType serverType)
{
    RoNetTcpConnectionPtr& serverConnection = mConnections[as_integer(serverType)];
    if (serverConnection)
    {
        serverConnection->close();
        serverConnection.reset();
    }
}
