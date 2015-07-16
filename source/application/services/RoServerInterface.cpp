#include "RoServerInterface.h"
#include <core/RoException.h>
#include <core/RoErrorInfo.h>
#include <core/RoStringUtil.h>
#include <core/RoLog.h>

#include <network/RoNetworkManager.h>
#include <network/events/RoServerConnectedEvent.h>
#include <network/events/RoServerConnectRequestFailedEvent.h>
#include <network/events/RoServerDisconnectedEvent.h>

#include <tbb/tbb_thread.h>

void RoServerInterface::initialize()
{
    addTaskHandler<RoServerConnectedEvent>(getServerEventName(ConnectionEvents::CONNECTED), &RoServerInterface::onConnectionSuccess);
    addTaskHandler<RoServerConnectRequestFailedEvent>(getServerEventName(ConnectionEvents::CONNECTION_FAILED), &RoServerInterface::onConnectionFailed);
    addTaskHandler<RoServerDisconnectedEvent>(getServerEventName(ConnectionEvents::DISCONNECTED), &RoServerInterface::onDisconnect);
}

void RoServerInterface::connect(const RoString& host, const uint16 port, const ConnectCallback& callback)
{
    RoString portNumberAsString = RoStringUtil::Format("%d", port);
    connect(host, portNumberAsString, callback);
}

void RoServerInterface::connect(const RoString& host, const RoString& portNumberAsString, const ConnectCallback& callback)
{
    if (isConnected())
    {
        mDisconnectCallback.reset();
        disconnect();
    }
    mConnectCallback.reset(callback);

    RoNetworkManager::Connect(mType, host, portNumberAsString);

    RoServerConnectionStatus currentStatus = getConnectionStatus();
    changeStatus(currentStatus, RoServerConnectionStatus::CONNECTING);
}

void RoServerInterface::asyncSendPacket(const RoPacketPtr packet)
{
    if (isNotConnected())
    {
        roTHROW(RoInvalidOperation() << RoErrorInfoDetail("Cannot send packets to server, when it's not connected."));
    }
    RoNetworkManager::SendToServer(mType, packet);
}

void RoServerInterface::disconnect()
{
    if (isNotConnected())
    {
        return;
    }
    RoNetworkManager::Disconnect(mType);
}

RoServerInterface::RoServerInterface(const RoNetServerType serverType)
    : mType{ serverType }
    , mConnectionStatus{ RoServerConnectionStatus::DISCONNECTED }
{
}

void RoServerInterface::onConnectionSuccess(const RoServerConnectedEvent& event)
{
    roLOG_INFO << "Connected to server " << roNetServerTypeToString(mType);
    RoServerConnectionStatus status = RoServerConnectionStatus::CONNECTING;
    changeStatus(status, RoServerConnectionStatus::CONNECTED);
    if (!mConnectCallback.is_initialized())
    {
        return;
    }
    RoOptionalString errorMessage;
    ConnectCallback& callback = mConnectCallback.get();
    callback(mType, errorMessage);
}


void RoServerInterface::onConnectionFailed(const RoServerConnectRequestFailedEvent& event)
{
    roLOG_ERR << "FAILED to connect to server " << roNetServerTypeToString(mType);
    RoServerConnectionStatus status = RoServerConnectionStatus::CONNECTING;
    changeStatus(status, RoServerConnectionStatus::DISCONNECTED);
    if (!mConnectCallback.is_initialized())
    {
        return;
    }
    RoOptionalString errorMessage = event.reason;
    ConnectCallback& callback = mConnectCallback.get();
    callback(mType, errorMessage);
}


void RoServerInterface::onDisconnect(const RoServerDisconnectedEvent& event)
{
    roLOG_INFO << "Disconnected from server " << roNetServerTypeToString(mType);
    RoServerConnectionStatus status = RoServerConnectionStatus::DISCONNECTING;
    changeStatus(status, RoServerConnectionStatus::DISCONNECTED);
    if (!mDisconnectCallback.is_initialized())
    {
        return;
    }
    DisconnectCallback& callback = mDisconnectCallback.get();
    callback(mType);
}

bool RoServerInterface::changeStatus(RoServerConnectionStatus& currentStatus, const RoServerConnectionStatus newStatus)
{
    const RoServerConnectionStatus expectedStatus = currentStatus;
    if (mConnectionStatus.compare_exchange_strong(currentStatus, newStatus))
    {
        return true;
    }
    roLOG_ERR
        << "Unexpected connection status change from "
        << to_string(currentStatus)
        << " to "
        << to_string(newStatus)
        << ". Found "
        << to_string(currentStatus)
        << " instead.";
    return false;
}

RoString RoServerInterface::getServerEventName(const ConnectionEvents event) const
{
    const RoString eventPrefix = getServerEventPrefix();
    switch (event)
    {
    case ConnectionEvents::CONNECTED:
        return eventPrefix + L"ServerConnected";
    case ConnectionEvents::DISCONNECTED:
        return eventPrefix + L"ServerDisconnected";
    case ConnectionEvents::CONNECTION_FAILED:
        return eventPrefix + L"ServerConnectionFailed";
    }
    roTHROW(RoInvalidOperation() << RoErrorInfoDetail("Invalid server event!"));
}

RoString RoServerInterface::getServerEventPrefix() const
{
    switch (mType)
    {
    case RoNetServerType::LOGIN:
        return RoString{ L"Login" };
    case RoNetServerType::CHARACTER:
        return RoString{ L"Character" };
    case RoNetServerType::MAP:
        return RoString{ L"Map" };
    default:
        break;
    }
    roTHROW(RoInvalidOperation() << RoErrorInfoDetail("Unknown server type."));
}

std::string to_string(const RoServerConnectionStatus connectionStatus)
{
    switch (connectionStatus)
    {
    case RoServerConnectionStatus::DISCONNECTED:
        return "DISCONNECTED";
    case RoServerConnectionStatus::CONNECTING:
        return "CONNECTING";
    case RoServerConnectionStatus::CONNECTED:
        return "CONNECTED";
    case RoServerConnectionStatus::DISCONNECTING:
        return "DISCONNECTING";
    default:
        break;
    }
    return "UNKNOWN";
}

std::ostream& operator<<(std::ostream& stream, const RoServerConnectionStatus connectionStatus)
{
    return stream << to_string(connectionStatus);
}
