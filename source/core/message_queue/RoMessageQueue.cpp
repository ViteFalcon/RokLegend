////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoMessageQueue.h"
#include "RoMessage.h"

#include <core/RoErrorInfo.h>
#include <core/RoException.h>
#include <core/RoLog.h>
#include <core/task/RoTaskManager.h>

#define roDEBUG_MESSAGE_QUEUE 0

#if defined(roDEBUG_MESSAGE_QUEUE) && roDEBUG_MESSAGE_QUEUE
#   define roMQ_LOG_DBG(msg) roLOG_DBG << msg
#else
#   define roMQ_LOG_DBG(msg)
#endif // roDEBUG_MESSAGE_QUEUE

RoMessageQueuePtr RoMessageQueue::sQueues[RoMessageQueue::eMessageQueue_Count] = {
    RoMessageQueuePtr(new RoMessageQueue("Graphics", "^Graphics.*")),
    RoMessageQueuePtr(new RoMessageQueue("Network", "^Network.*")),
    RoMessageQueuePtr(new RoMessageQueue("Default", ".*"))
};

RoMessageQueue::RoMessageQueue(const RoString& name, const RoString& regexFilter)
    : mName(name)
    , mRegex(regexFilter.asUTF8())
{
}

RoMessageQueue::~RoMessageQueue()
{
}

void RoMessageQueue::dispatch()
{
    RoMessagePtr message;
    while (mMessages.try_pop(message))
    {
        roRUN_TASK_NAMED(message->getFunction(), message->getArgs());
    }
}

RoMessageQueue& RoMessageQueue::Get(RoMessageQueue::Types type)
{
    if (type >= eMessageQueue_Count || type < 0)
    {
        roTHROW(RoInvalidOperation()
            << RoErrorInfoDetail("Cannot access a queue that is not a valid queue-type.")
            << RoErrorInfoHint("Did you use an integer instead of RoMessageQueue::Types enum?"));
    }
    return *sQueues[type];
}

void RoMessageQueue::_ClearAll()
{
    for (int i = 0; i < eMessageQueue_Count; ++i)
    {
        sQueues[i]->mMessages.clear();
    }
}

bool RoMessageQueue::Append(const RoString& function, const RoTaskArgs& args)
{
    RoMessagePtr message(new RoMessage{ function, args });
    for (int i = 0; i < eMessageQueue_Count; ++i)
    {
        if (sQueues[i]->enqueueIfAcceptable(message))
        {
            return true;
        }
    }

    roLOG_WARN << "Failed to append message '" << function << "'!";

    return false;
}

void RoMessageQueue::enqueue(const RoMessagePtr& message)
{
    mMessages.push(message);
    roMQ_LOG_DBG("Enqueued message '" << message->getFunction() << "' into " << mName << " queue.");
}

inline bool RoMessageQueue::enqueueIfAcceptable(const RoMessagePtr message)
{
    if (boost::regex_match(message->getFunction().asUTF8(), mRegex))
    {
        enqueue(message);
        return true;
    }
    return false;
}
