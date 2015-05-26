#pragma once

#include "../RoSharedPtr.h"

struct RoTaskArgs;
roDEFINE_PTR(RoTaskArgs);

/**
 * Base class for arguments for tasks
 */
struct RoTaskArgs
{
    RoTaskId taskId{ -1 };
    virtual RoTaskArgsPtr clone(const RoTaskId taskId) const = 0;
};

/**
 * Base class that provides a default cloning behavior via CRTP. This pattern expects that there is only a single level
 * of deriving.
 */
template <typename DerivedType>
struct RoTaskArgsBase : RoTaskArgs
{
    virtual RoTaskArgsPtr clone(const RoTaskId taskId) const override
    {
        auto val = dynamic_cast<const DerivedType *>(this);
        auto clonedArgs = std::make_shared<DerivedType>(*val);
        clonedArgs->taskId = taskId;
        return clonedArgs;
    }
};

struct RoEmptyArgs : public RoTaskArgsBase < RoEmptyArgs >
{
    static const RoEmptyArgs INSTANCE;
};