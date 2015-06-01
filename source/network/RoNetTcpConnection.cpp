////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////



#include "RoNetBufferDataStream.h"
#include "RoNetTcpConnection.h"
#include "RoNetPacketDb.h"
#include "RoNetServer.h"
#include "RoNetworkManager.h"

#include "events/RoServerConnectedEvent.h"
#include "events/RoServerConnectRequestFailedEvent.h"
#include "events/RoServerDisconnectedEvent.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#define roDEBUG_TCP_CONNECTION 0

#if defined(roDEBUG_TCP_CONNECTION) && roDEBUG_TCP_CONNECTION
#   define roTCP_LOG_DBG(msg) roLOG_DBG << msg
#else
#   define roTCP_LOG_DBG(msg)
#endif

#define roHEARTBEAT_INTERVAL_SECS 50
#define roCHECK_ASIO_ERROR_AND_DO(error, doLambda) handleError(error, __FUNCTION__, doLambda)
#define roCHECK_ASIO_ERROR(error) handleError(error, __FUNCTION__)

RoNetTcpConnection::RoNetTcpConnection(RoNetworkManager& parent, RoNetPacketDb& packetDb, RoNetServerType type, const RoString& server, const RoString& port)
    : mParent(parent)
    , mPacketDb(packetDb)
    , mType(type)
    , mResolver(mService)
    , mSocket(mService)
    , mKeepAliveTimer(mService)
    , mAlivePacket(packetDb.getHeartBeatPacket(type))
    , mConnected(true)
{
    ResolverQuery query(server, port);
    mResolver.async_resolve(query,
        boost::bind(&RoNetTcpConnection::handleResolve, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::iterator));
    mThreads.create_thread(boost::bind(&IoService::run, &mService));
    mPostMessage.store(true);
}

RoNetTcpConnection::~RoNetTcpConnection()
{
    mPostMessage.store(false);
    close();
}

void RoNetTcpConnection::close()
{
    if (mConnected)
    {
        mService.post(boost::bind(&RoNetTcpConnection::doClose, this));
        mThreads.join_all();
    }
}

void RoNetTcpConnection::doClose()
{
    mKeepAliveTimer.cancel();
    mTimerService.stop();
    if (mConnected)
    {
        roLOG_INFO << "Closing connection!";
        mConnected = false;
        mSocket.close();
        RoPropertyMap args;
        args.set("ServerType", mType);
        if (mPostMessage.load())
        {
            roPOST_MSG(NetworkServerDisconnected, args);
        }
    }
    mService.stop();
}

void RoNetTcpConnection::send(RoNetPacketPtr packet)
{
    RoNetBufferDataStream* dataStream = new RoNetBufferDataStream();
    packet->writeToStream(*dataStream);
    asyncWriteData(dataStream->data());
    delete dataStream;
    roTCP_LOG_DBG("Queued '" << packet->getActionName() << "' to send");
}

void RoNetTcpConnection::expectNonStandardPacketWithUint32PacketId(uint32 value, const RoString& action, const RoPropertyMap& args)
{
    if (!mUint32PacketIdCallbacks.insert(std::make_pair(value, std::make_pair(action, args))))
    {
        roLOG_ERR
            << "Failed to add non-standard packet-id response information that calls back on action '"
            << action << "' to the map. This may cause some issues.";
    }
}

void RoNetTcpConnection::handleResolve(const SystemErrorCode& error, ResolverItr resolverIterator)
{
    roCHECK_ASIO_ERROR(error);
    boost::asio::async_connect(mSocket, resolverIterator,
        boost::bind(&RoNetTcpConnection::handleConnect, this,
            boost::asio::placeholders::error));
}

void RoNetTcpConnection::handleConnect(const SystemErrorCode& error)
{
    roCHECK_ASIO_ERROR_AND_DO(error, [&]() {
        RoServerConnectRequestFailedEvent event;
        event.serverType = mType;
        event.reason = error.message();
        roPOST_MSG(NetworkServerConnectFailed, event);
        mConnected = false;
    });
    mConnected = true;
    RoPropertyMap args;
    args.set("ServerType", mType);
    roPOST_MSG(NetworkServerConnected, args);
    doKeepAlive();
    asyncReadPacket();
}

roINLINE void RoNetTcpConnection::asyncReadPacket()
{
    if (!mConnected)
        return;
    boost::asio::async_read(mSocket, boost::asio::buffer(reinterpret_cast<char*>(&mPacketId), 2),
        boost::asio::transfer_exactly(sizeof(PacketId)),
        boost::bind(&RoNetTcpConnection::handleReadPacket, this,
            boost::asio::placeholders::error));
}

void RoNetTcpConnection::handleReadPacket(const SystemErrorCode& error)
{
    roCHECK_ASIO_ERROR(error);
    doReadPacket();
    asyncReadPacket();
}

void RoNetTcpConnection::doReadPacket()
{
    if (0 == mPacketId)
    {
        return;
    }
    if (mPacketDb.hasPacket(mPacketId))
    {
        SocketDataStream* socketStream = new SocketDataStream(mSocket);
        socketStream->advance(2);
        RoNetPacketPtr packet = mPacketDb.getPacketById(mPacketId);
        packet->tryReadFromStream(*socketStream);
        delete socketStream;
        roPOST_MSG_NAMED(packet->getActionName(), packet->getProperties());
    }
    else if (!mUint32PacketIdCallbacks.empty())
    {
        PacketId unknownPacketId = mPacketId;
        SocketDataStream* socketStream = new SocketDataStream(mSocket);
        uint32 val = 0;
#ifdef WIN32
        memcpy_s(&val, sizeof(uint32), &mPacketId, sizeof(PacketId));
#else
        memcpy(&val, &mPacketId, sizeof(PacketId));
#endif
        socketStream->read(&mPacketId, sizeof(PacketId));
#ifdef WIN32
        memcpy_s(reinterpret_cast<char*>(&val)+sizeof(PacketId), sizeof(uint32)-sizeof(PacketId), &mPacketId, sizeof(PacketId));
#else
        memcpy(reinterpret_cast<char*>(&val)+sizeof(PacketId), &mPacketId, sizeof(PacketId));
#endif
        Uint32PacketIdCallbackMap::const_accessor uint32PacketIdIterator;
        if (mUint32PacketIdCallbacks.find(uint32PacketIdIterator, mPacketId))
        {
            RoMessageQueue::Append(uint32PacketIdIterator->second.first, uint32PacketIdIterator->second.second);
        }
        else
        {
            roLOG_CRIT
                << "Failed to deduce the packet type for packet-id 0x"
                << std::hex << unknownPacketId << std::dec << ". "
                << "This will corrupt rest of the network stream!";
        }
    }
    mPacketId = 0;
}

void RoNetTcpConnection::asyncWriteData(const RoBuffer& data)
{
    mDataQueue.push(data);
    scheduleWrite();
}

roINLINE void RoNetTcpConnection::scheduleWrite()
{
    RoBuffer data;
    if (mConnected && mDataQueue.try_pop(data))
    {
        //
        boost::asio::async_write(mSocket,
            boost::asio::buffer(&data[0], data.size()),
            boost::bind(&RoNetTcpConnection::handleWrite, this,
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
}

void RoNetTcpConnection::handleWrite(const SystemErrorCode& error, RoStreamSize bytesTransferred)
{
    roCHECK_ASIO_ERROR(error);
    scheduleWrite();
}

void RoNetTcpConnection::doKeepAlive()
{
    if (mConnected && mAlivePacket)
    {
        mKeepAliveTimer.expires_from_now(boost::posix_time::seconds(roHEARTBEAT_INTERVAL_SECS));
        mKeepAliveTimer.async_wait(
            boost::bind(&RoNetTcpConnection::handleHeartBeat, this,
                boost::asio::placeholders::error));
        mThreads.create_thread(boost::bind(&boost::asio::io_service::run, &mTimerService));
    }
}

void RoNetTcpConnection::handleHeartBeat(const SystemErrorCode& error)
{
    roCHECK_ASIO_ERROR(error);
    send(mAlivePacket);
    doKeepAlive();
}

inline void RoNetTcpConnection::handleError(const SystemErrorCode& error, const char* functionName)
{
    AdditionalErrorActions errorActions;
    handleError(error, functionName, errorActions);
}

inline void RoNetTcpConnection::handleError(const SystemErrorCode& error, const char* functionName, AdditionalErrorActions additionalActions)
{
    if (error && mConnected)
    {
        roLOG_ERR << error.message() << " @ " << functionName;
        if (additionalActions)
        {
            additionalActions();
        }
        doClose();
    }
}
