#pragma once
#include <core/RoPrerequisites.h>
#include <core/task/RoTaskHandler.h>

#include <network/RoNetServerType.h>
#include <network/packets/RoPacket.h>

#include <ostream>

struct RoServerConnectedEvent;
struct RoServerConnectRequestFailedEvent;
struct RoServerDisconnectedEvent;

enum class RoServerConnectionStatus
{
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    DISCONNECTING
};

class RoServerInterface : public RoTaskHandler < RoServerInterface >
{
public:
    /**
     ** Callback to notify about connection status.
     ** @param RoNetServerType: Type of the server
     ** @param RoOptionalString: Error message (if any).
     **/
    using ConnectCallback = std::function < void(RoNetServerType, RoOptionalString) > ;
    using DisconnectCallback = std::function < void(RoNetServerType) > ;

    RoServerInterface(const RoNetServerType serverType);
    virtual ~RoServerInterface() = default;

    void initialize();

    RoServerConnectionStatus getConnectionStatus() const
    {
        return mConnectionStatus.load();
    }

    bool isConnected() const
    {
        return getConnectionStatus() == RoServerConnectionStatus::CONNECTED;
    }

    bool isNotConnected() const
    {
        return getConnectionStatus() != RoServerConnectionStatus::CONNECTED;
    }

    void connect(const RoString& host, const uint16 port, const ConnectCallback& callback);
    void connect(const RoString& host, const RoString& portNumberAsString, const ConnectCallback& callback);

    void asyncSendPacket(const RoPacketPtr packet);

    void disconnect();

private: // task handlers
    void onConnectionSuccess(const RoServerConnectedEvent& event);
    void onConnectionFailed(const RoServerConnectRequestFailedEvent& event);
    void onDisconnect(const RoServerDisconnectedEvent& event);

    bool changeStatus(RoServerConnectionStatus& expectedStatus, const RoServerConnectionStatus newStatus);

    enum class ConnectionEvents
    {
        CONNECTED,
        CONNECTION_FAILED,
        DISCONNECTED
    };
    RoString getServerEventName(const ConnectionEvents event) const;
    RoString getServerEventPrefix() const;

    const RoNetServerType mType;
    RoAtomic<RoServerConnectionStatus> mConnectionStatus;
    optional<ConnectCallback> mConnectCallback;
    optional<DisconnectCallback> mDisconnectCallback;
};

std::string to_string(const RoServerConnectionStatus connectionStatus);
std::ostream& operator << (std::ostream& stream, const RoServerConnectionStatus connectionStatus);

template <typename DerivedType>
class RoServerInterfaceT : public RoTaskHandler < DerivedType >
{
public:
    using ConnectCallback = RoServerInterface::ConnectCallback;
    using DisconnectCallback = RoServerInterface::DisconnectCallback;

    RoServerInterfaceT(const RoNetServerType type)
        : mInterface(type)
    {
    }
    virtual ~RoServerInterfaceT() = default;

    void initialize()
    {
        static bool shouldInitialize = true;
        if (shouldInitialize)
        {
            mInterface.initialize();
            addTaskHandlers();
            shouldInitialize = false;
        }
    }

    RoServerConnectionStatus getConnectionStatus() const
    {
        return mConnectionStatus.load();
    }

    bool isConnected() const
    {
        return mInterface.isConnected();
    }

    bool isNotConnected() const
    {
        return mInterface.isNotConnected();
    }

    void connect(const RoString& host, const uint16 port, const ConnectCallback& callback)
    {
        mInterface.connect(host, port, callback);
    }

    void connect(const RoString& host, const RoString& portNumberAsString, const ConnectCallback& callback)
    {
        mInterface.connect(host, portNumberAsString, callback);
    }

    void asyncSendPacket(const RoPacketPtr packet)
    {
        mInterface.asyncSendPacket(packet);
    }

    void disconnect()
    {
        mInterface.disconnect();
    }

protected:
    virtual void addTaskHandlers() = 0;

private:
    RoServerInterface mInterface;
};
