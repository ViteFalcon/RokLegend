#pragma once
#include "../RoPrerequisites.h"
#include "../RoSharedPtr.h"
#include "RoTask.h"

class RoTaskManager
{
public: // Static
    static RoTaskManager& GetInstance();

    virtual void registerTask(const std::string& taskName, const RoTask& task) = 0;
    virtual void unregisterTask(const std::string& taskName) = 0;

    virtual void run(const std::string& taskName) const = 0;
    virtual void run(const std::string& taskName, const RoTaskArgs& args) const = 0;

    virtual void schedule(const std::string& taskName) const = 0;
    virtual void schedule(const std::string& taskName, const RoTaskArgs& args) const = 0;
protected:
    RoTaskManager() = default;
    virtual ~RoTaskManager() = default;

    RoTaskManager(const RoTaskManager&) = delete;
    RoTaskManager& operator= (const RoTaskManager&) = delete;
};

#define roSCHEDULE_TASK(taskname, args) RoTaskManager::GetInstance().schedule(roSTRINGIFY(taskname), args)
#define roRUN_TASK(taskname, args) RoTaskManager::GetInstance().run(roSTRINGIFY(taskname), args)
#define roRUN_TASK_NAMED(taskNameString, args) RoTaskManager::GetInstance().run(taskNameString, args)