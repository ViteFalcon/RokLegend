#pragma once

#include "RoTaskCollection.h"
#include <core/RoException.h>
#include <core/RoErrorInfo.h>

template <typename DerivedType>
class RoTaskHandler
{
protected:
    RoTaskHandler() = default;
    virtual ~RoTaskHandler() = default;

    class RoTaskExecutor
    {
    public:
        RoTaskExecutor() = default;
        virtual ~RoTaskExecutor() = default;

        virtual void run(const RoTaskArgs& args) = 0;

        RoTask getTask()
        {
            return std::bind(&RoTaskExecutor::run, this, std::placeholders::_1);
        }
    };

    using RoTaskExecutorCollection = RoVector < RoSharedPtr<RoTaskExecutor> > ;

    template <typename ArgType>
    class RoTaskExecutorHelper : public RoTaskExecutor
    {
    public:
        typedef void(DerivedType::*Type)(const ArgType&);
        using CallbackType = std::function < void(const ArgType&) > ;

        RoTaskExecutorHelper(DerivedType* object, Type method)
            : mCallback(std::bind(method, object, std::placeholders::_1))
        {
            static_assert(std::is_base_of<RoTaskArgs, ArgType>::value, "ArgType should inherit from RoTaskArgs for the task.");
        }

        virtual void run(const RoTaskArgs& args) override
        {
            mCallback(as_event_args<ArgType>(args));
        }

    private:
        CallbackType mCallback;
    };
    typedef void (DerivedType::*TaskFunction)(const RoTaskArgs&);

    void add(const RoString& name, TaskFunction taskFunction)
    {
        auto self = getSelf();
        mTasks.add(name, std::bind(taskFunction, self, std::placeholders::_1));
    }

    template <typename T>
    void add(const RoString& name, typename RoTaskExecutorHelper<T>::Type taskFunction)
    {
        auto self = getSelf();
        auto executor = std::make_shared<RoTaskExecutorHelper<T>>(self, taskFunction);
        mExecutors.push_back(executor);
        mTasks.add(name, executor->getTask());
    }
private:
    DerivedType* getSelf()
    {
        static_assert(std::is_base_of<RoTaskHandler<DerivedType>, DerivedType>::value, "DerivedType should inherit from RoTaskHandler<DerivedType>!");
        return dynamic_cast<DerivedType*>(this);
    }
    RoTaskCollection mTasks;
    RoTaskExecutorCollection mExecutors;
};
