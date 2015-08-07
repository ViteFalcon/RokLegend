#include "RoTaskManager.h"
#include "../RoException.h"
#include "../RoErrorInfo.h"
#include "../RoLog.h"

#include <tbb/concurrent_hash_map.h>
#include <tbb/task.h>

class RoTbbTask : public tbb::task
{
public:
    RoTbbTask(const RoTaskId id, const RoTask& task, const RoTaskArgs& args)
        : mId(id)
        , mTask(task)
        , mArgs(args.clone(id))
    {
    }

    virtual tbb::task* execute() override
    {
        mTask(*mArgs);
        return nullptr;
    }

    RoTaskId getId() const
    {
        return mId;
    }
private:
    const RoTaskId mId;
    const RoTask mTask;
    const RoTaskArgsPtr mArgs;
};

class RoTaskManagerImpl : public RoTaskManager
{
public:
    RoTaskManagerImpl()
    {
        sNextId = 1ULL;
    }
    ~RoTaskManagerImpl() = default;

    virtual void registerTask(const std::string& taskName, const RoTask& task) override
    {
        mTasks.insert(TaskInfoMap::value_type{ taskName, task });
    }
    virtual void unregisterTask(const std::string& taskName) override
    {
        TaskInfoMap::accessor accessor;
        if (!mTasks.find(accessor, taskName))
        {
            roLOG_ERR << "Failed to find task named '" << taskName << "' to remove it.";
            return;
        }
        if (!mTasks.erase(accessor))
        {
            roLOG_ERR << "Failed to erase task named '" << taskName << "'.";
        }
    }

    virtual void run(const std::string& taskName) const override
    {
        run(taskName, RoEmptyArgs::INSTANCE);
    }
    virtual void run(const std::string& taskName, const RoTaskArgs& args) const override
    {
        RoTask task = getTaskNamed(taskName);
        task(args);
    }

    virtual void schedule(const std::string& taskName) const override
    {
        schedule(taskName, RoEmptyArgs::INSTANCE);
    }
    virtual void schedule(const std::string& taskName, const RoTaskArgs& args) const override
    {
        RoTaskId taskId = sNextId.fetch_and_add(1);
        RoTask task = getTaskNamed(taskName);
        RoTbbTask& tbbTask = *new (tbb::task::allocate_root()) RoTbbTask(taskId, task, args);
        tbb::task::spawn(tbbTask);
    }

    RoTask getTaskNamed(const std::string& taskName) const
    {
        TaskInfoMap::const_accessor accessor;
        if (!mTasks.find(accessor, taskName))
        {
            roTHROW(RoItemNotFound() << RoErrorInfoItemName(taskName));
        }
        return accessor->second;
    }
private: // Static fields
    using RoAtomicTaskId = tbb::atomic < RoTaskId > ;
    static RoAtomicTaskId sNextId;

private: // Fields
    using TaskInfoMap = tbb::concurrent_hash_map < std::string, RoTask > ;

    TaskInfoMap mTasks;
};

RoTaskManagerImpl::RoAtomicTaskId RoTaskManagerImpl::sNextId;

RoTaskManager& RoTaskManager::GetInstance()
{
    static RoTaskManagerImpl impl;
    return impl;
}