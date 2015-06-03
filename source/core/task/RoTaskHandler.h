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

    typedef void (DerivedType::*TaskFunction)(const RoTaskArgs&);

    void add(const RoString& name, TaskFunction taskFunction)
    {
        static_assert(std::is_base_of<RoTaskHandler<DerivedType>, DerivedType>::value, "DerivedType should inherit from RoTaskHandler<DerivedType>!");
        DerivedType* self = dynamic_cast<DerivedType*>(this);
        roTHROW_IF(nullptr == self, RoInvalidOperation() << RoErrorInfoDetail("Failed to convert task-handler type to derived-type."));
        mTasks.add(name, std::bind(taskFunction, self, std::placeholders::_1));
    }
private:
    RoTaskCollection mTasks;
};

#define roBOUND_TASK(taskName, method) add(roSTRINGIFY(taskName), method)
