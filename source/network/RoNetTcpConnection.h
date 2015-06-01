////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_NET_TCP_CONNECTION_H
#define ROKLEGEND_NET_TCP_CONNECTION_H

#include <core/RoPrerequisites.h>
#include <core/RoPropertyMap.h>

#include "RoNetPacket.h"
#include "RoNetServerType.h"
#include "RoNetSocketInputStream.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_queue.h>

class RoNetworkManager;
class RoNetPacketDb;

/**
  * Brief description about RoNetTcpConnection.
 **/
class RoNetTcpConnection
{
public:
    typedef boost::asio::io_service                             IoService;
    typedef boost::asio::streambuf                              StreamBuffer;
    typedef boost::asio::deadline_timer                         Timer;
    typedef boost::asio::ip::tcp                                Protocol;
    typedef Protocol::socket                                    Socket;
    typedef Protocol::resolver                                  Resolver;
    typedef Resolver::iterator                                  ResolverItr;
    typedef Resolver::query                                     ResolverQuery;
    typedef boost::system::error_code                           SystemErrorCode;
    typedef uint16                                              PacketId;
    typedef boost::thread                                       Thread;
    typedef RoNetSocketInputStream<Socket, StreamBuffer>        SocketDataStream;
    typedef tbb::concurrent_queue<RoBuffer>                     DataQueue;
    typedef std::pair<RoString, RoPropertyMap>                  ActionArgsPair;
    typedef tbb::concurrent_hash_map<uint32, ActionArgsPair>    Uint32PacketIdCallbackMap;
    typedef std::function<void()>                               AdditionalErrorActions;

    RoNetTcpConnection(RoNetworkManager& parent, RoNetPacketDb& packetDb, RoNetServerType type, const RoString& server, const RoString& port);
    virtual ~RoNetTcpConnection();

    inline RoNetServerType getServerType() const
    {
        return mType;
    }

    void close();

    void send(RoNetPacketPtr packet);

    void expectNonStandardPacketWithUint32PacketId(uint32 value, const RoString& action, const RoPropertyMap& args = RoPropertyMap());

private: // Functions
    void handleResolve(const SystemErrorCode& error, ResolverItr resolverIterator);
    void handleConnect(const SystemErrorCode& error);
    void handleReadPacket(const SystemErrorCode& error);
    void handleWrite(const SystemErrorCode& error, RoStreamSize bytesTransferred);
    void handleHeartBeat(const SystemErrorCode& error);
    void handleError(const SystemErrorCode& error, const char* functionName);
    void handleError(const SystemErrorCode& error, const char* functionName, AdditionalErrorActions additionalActions);

    void asyncReadPacket();
    void asyncWriteData(const RoBuffer& data);
    void scheduleWrite();

    void doClose();
    void doKeepAlive();
    void doReadPacket();
    void doWrite(const RoBuffer& data);

private: // Members
    RoNetworkManager&           mParent;
    RoNetPacketDb&              mPacketDb;
    const RoNetServerType       mType;
    IoService                   mService;
    IoService                   mTimerService;
    Resolver                    mResolver;
    Socket                      mSocket;
    Timer                       mKeepAliveTimer;
    PacketId                    mPacketId;
    RoNetPacketPtr              mAlivePacket;
    boost::thread_group         mThreads;
    Uint32PacketIdCallbackMap   mUint32PacketIdCallbacks;
    DataQueue                   mDataQueue;
    bool                        mConnected;
    RoAtomicBool                mPostMessage;
};

#endif // ROKLEGEND_NET_TCP_CONNECTION_H
