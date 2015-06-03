////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NETWORK_MANAGER_H
#define ROKLEGEND_NETWORK_MANAGER_H

#include <core/RoPrerequisites.h>

#include "RoNetServerType.h"

#include <core/message_queue/RoMessageQueue.h>
#include <core/task/RoTaskHandler.h>

class RoNetPacketDb;
roDEFINE_PTR(RoNetPacketDb);

class RoNetTcpConnection;
roDEFINE_PTR(RoNetTcpConnection);

class RoNetTcpConnection;

/**
  * Brief description about RoNetworkManager.
 **/
class RoNetworkManager : public RoTaskHandler<RoNetworkManager>
{
public:
    static void Connect(RoNetServerType serverType, const RoString& ipAddress, const RoString& portNumber);
    static void Disconnect(RoNetServerType serverType);

    RoNetworkManager(const RoString& packetDb);
    virtual ~RoNetworkManager();


    void _setConnected(RoNetServerType type, RoNetTcpConnection* connection){}
    void _setDisconnected(RoNetServerType type, RoNetTcpConnection* connection){}


private: // Tasks
    void update(const RoTaskArgs&);
    void sendToLoginServer(const RoTaskArgs& args);
    void sendToCharacterServer(const RoTaskArgs& args);
    void sendToMapServer(const RoTaskArgs& args);
    void serverConnected(const RoTaskArgs& args);
    void serverConnectionFailed(const RoTaskArgs& args);
    void serverDisconnected(const RoTaskArgs& args);
    void connect(const RoTaskArgs& args);
    void disconnect(const RoTaskArgs& args);
    void loginServerConnected(const RoTaskArgs& args);
    void loginServerDisconnected(const RoTaskArgs& args);
    void characterServerConnected(const RoTaskArgs& args);
    void characterServerDisconnected(const RoTaskArgs& args);
    void mapServerConnected(const RoTaskArgs& args);
    void mapServerDisconnected(const RoTaskArgs& args);

private: // Functions
    void connectToServer(RoNetServerType serverType, const RoString& host, const RoString& port);
    void disconnectFrom(RoNetServerType serverType);

private:
    RoMessageQueue&         mMessages;
    RoNetPacketDbPtr        mPacketDb;
    RoNetTcpConnectionPtr   mConnections[(int)RoNetServerType::COUNT];
    RoAtomicBool            mOkToUpdate;
};

#endif // ROKLEGEND_NETWORK_MANAGER_H
