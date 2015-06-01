////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_MESSAGE_QUEUE_H
#define ROKLEGEND_MESSAGE_QUEUE_H

#include "../RoPrerequisites.h"
#include "../RoPropertyMap.h"

#include "RoMessage.h"

#include <boost/regex.hpp>
#include <boost/function.hpp>

#include <tbb/concurrent_queue.h>

class RoMessageQueue;
roDEFINE_PTR(RoMessageQueue);

/**
  * Brief description about RoMessageQueue.
 **/
class RoMessageQueue
{
public:
    enum Types
    {
        eMessageQueue_Graphics,
        eMessageQueue_Network,
        // Default should always be at the end of the enumerations as it accepts any message that doesn't fit
        // in the queues above it.
        eMessageQueue_Default,

        eMessageQueue_Count
    };

    virtual ~RoMessageQueue();

    bool empty()
    {
        return mMessages.empty();
    }

    void dispatch();

public: // Static
    static RoMessageQueue& Get(RoMessageQueue::Types type);

    static bool Append(const RoString& function, const RoPropertyMap& args);

    static void _ClearAll();
private: // Functions
    RoMessageQueue(const RoString& name, const RoString& regexFilter);
    RoMessageQueue(const RoMessageQueue&) = delete;
    RoMessageQueue& operator = (const RoMessageQueue&) = delete;

    void enqueue(const RoMessagePtr& message);

    bool enqueueIfAcceptable(const RoMessagePtr message);

private: // Static Members
    static RoMessageQueuePtr sQueues[eMessageQueue_Count];

private: // Members
    typedef tbb::concurrent_queue<RoMessagePtr> Queue;

    const RoString        mName;
    const boost::regex  mRegex;
    Queue               mMessages;
};

//#define roPOST_MSG(function, args) RoMessageQueue::Append(roSTRINGIFY(function), args)
//#define roPOST_MSG_NAMED(functionName, args) RoMessageQueue::Append(functionName, args)
#define roPOST_MSG_NAMED(functionName, args)

#endif // ROKLEGEND_MESSAGE_QUEUE_H
