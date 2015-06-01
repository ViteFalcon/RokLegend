#pragma once

#include "RoTaskCollection.h"

template <typename DerivedType>
class RoTaskHandler
{
protected:
    RoTaskHandler() = default;
    virtual ~RoTaskHandler() = default;

    typedef void (DerivedType::*TaskFunction)(const RoTaskArgs&);

    void add(const RoString& name, TaskFunction taskFunction)
    {
        DerivedType* self = dynamic_cast<DerivedType*>(this);
        mTasks.add(name, std::bind(taskFunction, self, std::placeholders::_1));
    }
private:
    RoTaskCollection mTasks;
};

#define roBOUND_TASK(taskName, method) add(roSTRINGIFY(taskName), method)
