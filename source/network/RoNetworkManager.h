////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnar�k Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NETWORK_MANAGER_H
#define ROKLEGEND_NETWORK_MANAGER_H

#include <core/RoPrerequisites.h>

#include "RoNetServerType.h"
#include "packets/RoPacket.h"

#include <core/message_queue/RoMessageQueue.h>
#include <core/task/RoTaskHandler.h>

class RoNetPacketDb;
roDEFINE_PTR(RoNetPacketDb);

class RoNetTcpConnection;
roDEFINE_PTR(RoNetTcpConnection);

class RoNetTcpConnection;

struct RoServerConnectRequestEvent;
struct RoServerDisconnectRequestEvent;
struct RoServerConnectedEvent;
struct RoServerConnectRequestFailedEvent;
struct RoServerDisconnectedEvent;
struct RoSendPacketToServerEvent;

/**
  * Brief description about RoNetworkManager.
 **/
class RoNetworkManager : public RoTaskHandler<RoNetworkManager>
{
public:
    static void Connect(RoNetServerType serverType, const RoString& ipAddress, const RoString& portNumber);
    static void ScheduleUpdate();
    static void SendToServer(RoNetServerType serverType, RoPacketPtr packet);
    static void Disconnect(RoNetServerType serverType);

    RoNetworkManager(const RoString& packetDb);
    virtual ~RoNetworkManager();


    void _setConnected(RoNetServerType type, RoNetTcpConnection* connection){}
    void _setDisconnected(RoNetServerType type, RoNetTcpConnection* connection){}


private: // Tasks
    void update(const RoTaskArgs&);
    void sendToLoginServer(const RoSendPacketToServerEvent& args);
    void sendToCharacterServer(const RoSendPacketToServerEvent& args);
    void sendToMapServer(const RoSendPacketToServerEvent& args);
    void sendToServer(const RoNetServerType serverType, const char* name, const RoSendPacketToServerEvent& args);
    void serverConnected(const RoServerConnectedEvent& args);
    void serverConnectionFailed(const RoServerConnectRequestFailedEvent& args);
    void serverDisconnected(const RoServerDisconnectedEvent& args);
    void connect(const RoServerConnectRequestEvent& args);
    void disconnect(const RoServerDisconnectRequestEvent& args);
    void loginServerConnected(const RoTaskArgs& args);
    void loginServerDisconnected(const RoTaskArgs& args);
    void characterServerConnected(const RoTaskArgs& args);
    void characterServerDisconnected(const RoTaskArgs& args);
    void mapServerConnected(const RoTaskArgs& args);
    void mapServerDisconnected(const RoTaskArgs& args);

private: // Functions
    void connectToServer(RoNetServerType serverType, const RoString& host, const RoString& port);
    void disconnectFrom(RoNetServerType serverType);

private: // static
    static const RoString NETWORK_UPDATE_TASK;

private:
    RoMessageQueue&         mMessages;
    RoNetPacketDbPtr        mPacketDb;
    RoNetTcpConnectionPtr   mConnections[(int)RoNetServerType::COUNT];
    RoAtomicBool            mOkToUpdate;
};

roDEFINE_PTR(RoNetworkManager);

#endif // ROKLEGEND_NETWORK_MANAGER_H
