#include "RoTaskCollection.h"
#include "RoTaskManager.h"

RoTaskCollection::~RoTaskCollection()
{
    clear();
}

void RoTaskCollection::add(const RoString& name, const RoTask& task)
{
    RoTaskManager::GetInstance().registerTask(name, task);
    mTaskNames.push_back(name);
}

void RoTaskCollection::clear()
{
    for (auto taskName : mTaskNames)
    {
        RoTaskManager::GetInstance().unregisterTask(taskName);
    }
    mTaskNames.clear();
}
