////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////


#include "RoNetServerStatus.h"

class RoNetServerStatusInfo
{
public:
    RoNetServerStatusInfo(RoNetServerStatus status, const RoString& statusString)
        : mStatus(status)
        , mString(statusString)
    {
    }

    virtual ~RoNetServerStatusInfo()
    {
    }

    RoNetServerStatus getStatus() const
    {
        return mStatus;
    }

    RoString toString() const
    {
        return mString;
    }

private:
    const RoNetServerStatus mStatus;
    const RoString& mString;
};

class RoServerDisconnectedStatusInfo : public RoNetServerStatusInfo
{
public:
    RoServerDisconnectedStatusInfo()
        : RoNetServerStatusInfo(eNetServerStatus_Disconnected, "Disconnected")
    {
    }
};

class RoServerConnectFailedStatusInfo : public RoNetServerStatusInfo
{
public:
    RoServerConnectFailedStatusInfo()
        : RoNetServerStatusInfo(eNetServerStatus_ConnectFailed, "ConnectFailed")
    {
    }
};

class RoServerConnectedStatusInfo : public RoNetServerStatusInfo
{
public:
    RoServerConnectedStatusInfo()
        : RoNetServerStatusInfo(eNetServerStatus_Connected, "Connected")
    {
    }
};

class RoNetServerStatusInfoFactory
{
public:
    static RoNetServerStatusInfo& Get(RoNetServerStatus serverStatus)
    {
        static RoServerDisconnectedStatusInfo disconnected;
        static RoServerConnectFailedStatusInfo connectFailed;
        static RoServerConnectedStatusInfo connected;

        switch(serverStatus)
        {
        case eNetServerStatus_ConnectFailed:
            return connectFailed;
        case eNetServerStatus_Connected:
            return connected;
        }
        return disconnected;
    }
};

RoString roNetServerStatusToString(RoNetServerStatus serverStatus)
{
    RoNetServerStatusInfo& statusInfo = RoNetServerStatusInfoFactory::Get(serverStatus);
    return statusInfo.toString();
}
