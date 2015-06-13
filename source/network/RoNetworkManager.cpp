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
#include <core/task/RoTaskManager.h>

#include "events/RoServerConnectRequestEvent.h"
#include "events/RoServerConnectRequestFailedEvent.h"
#include "events/RoServerDisconnectRequestEvent.h"
#include "events/RoServerConnectedEvent.h"
#include "events/RoServerDisconnectedEvent.h"
#include "events/RoSendPacketToServerEvent.h"
#include "events/RoPacketReceivedEvent.h"

const RoString RoNetworkManager::NETWORK_UPDATE_TASK{ L"_NetworkUpdate" };

void RoNetworkManager::Connect(RoNetServerType serverType, const RoString& ipAddress, const RoString& portNumber)
{
    RoServerConnectRequestEvent event;
    event.serverType = serverType;
    event.ipAddress = ipAddress;
    event.portNumber = portNumber;
    roPOST_MSG(NetworkServerConnect, event);
}

void RoNetworkManager::ScheduleUpdate()
{
    roSCHEDULE_TASK_NAMED(NETWORK_UPDATE_TASK, RoEmptyArgs::INSTANCE);
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
    roPOST_MSG(NetworkServerDisconnect, event);
}

RoNetworkManager::RoNetworkManager(const RoString &packetDb)
    : mMessages(RoMessageQueue::Get(RoMessageQueue::eMessageQueue_Network))
    , mPacketDb(RoNetPacketDb::LoadFromSource(packetDb))
{
    addTaskHandler(NETWORK_UPDATE_TASK, &RoNetworkManager::update);
    addTaskHandler<RoServerConnectRequestEvent>(L"NetworkServerConnect", &RoNetworkManager::connect);
    addTaskHandler<RoServerDisconnectRequestEvent>(L"NetworkServerDisconnect", &RoNetworkManager::disconnect);
    addTaskHandler<RoServerConnectedEvent>(L"NetworkServerConnected", &RoNetworkManager::serverConnected);
    addTaskHandler<RoServerConnectRequestFailedEvent>(L"NetworkServerConnectFailed", &RoNetworkManager::serverConnectionFailed);
    addTaskHandler<RoServerDisconnectedEvent>(L"NetworkServerDisconnected", &RoNetworkManager::serverDisconnected);
    addTaskHandler<RoSendPacketToServerEvent>(L"NetworkLoginServerSend", &RoNetworkManager::sendToLoginServer);
    addTaskHandler<RoSendPacketToServerEvent>(L"NetworkCharacterServerSend", &RoNetworkManager::sendToCharacterServer);
    addTaskHandler<RoSendPacketToServerEvent>(L"NetworkMapServerSend", &RoNetworkManager::sendToMapServer);

    mOkToUpdate.store(true);
}

RoNetworkManager::~RoNetworkManager()
{
    for (int i = 0; i < (int) RoNetServerType::COUNT; ++i)
    {
        mConnections[i].reset();
    }
}

void RoNetworkManager::sendToLoginServer(const RoSendPacketToServerEvent& args)
{
    sendToServer(RoNetServerType::LOGIN, "Login", args);
}

void RoNetworkManager::sendToCharacterServer(const RoSendPacketToServerEvent& args)
{
    sendToServer(RoNetServerType::CHARACTER, "Character", args);
}

void RoNetworkManager::sendToMapServer(const RoSendPacketToServerEvent& args)
{
    sendToServer(RoNetServerType::MAP, "Map", args);
}

void RoNetworkManager::sendToServer(const RoNetServerType serverType, const char* name, const RoSendPacketToServerEvent& args)
{
    RoNetTcpConnectionPtr& serverConnection = mConnections[as_integer(serverType)];
    if (!serverConnection)
    {
        roLOG_ERR << name << " server is not connected for this operation to be completed.";
        return;
    }
    RoNetPacketPtr packet = mPacketDb->getPacketByAction(args.packet->getActionName());
    packet->fromProperties(args.packet->getProperties());
    serverConnection->send(packet);
}

void RoNetworkManager::serverConnected(const RoServerConnectedEvent& args)
{
    RoNetServerActions::Get(args.serverType).onConnectionSuccess(args);
}

void RoNetworkManager::serverConnectionFailed(const RoServerConnectRequestFailedEvent& args)
{
    RoNetServerActions::Get(args.serverType).onConnectionFailure(args);
}

void RoNetworkManager::serverDisconnected(const RoServerDisconnectedEvent& args)
{
    mConnections[as_integer(args.serverType)].reset();
    RoNetServerActions::Get(args.serverType).onDisconnection(args);
}

void RoNetworkManager::connect(const RoServerConnectRequestEvent& args)
{
    connectToServer(args.serverType, args.ipAddress, args.portNumber);
}

void RoNetworkManager::disconnect(const RoServerDisconnectRequestEvent& args)
{
    disconnectFrom(args.serverType);
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
