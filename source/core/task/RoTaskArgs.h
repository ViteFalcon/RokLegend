#pragma once

#include "../RoSharedPtr.h"
#include "../RoPropertyMap.h"

struct RoTaskArgs;
roDEFINE_PTR(RoTaskArgs);

/**
 * Base class for arguments for tasks
 */
struct RoTaskArgs
{
    RoTaskId taskId{ 0ULL };

    RoTaskArgsPtr clone() const;
    virtual RoTaskArgsPtr clone(const RoTaskId taskId) const = 0;

    void set(const RoHashString& name, const RoVariant& val);

    template <typename T>
    roINLINE void set(const RoHashString &name, const T &val)
    {
        set(name, RoVariant(val));
    }

    template <typename T>
    void get(const RoHashString &name, T& val) const
    {
        return mProperties.getProperty(name).as(val);
    }

    inline void merge(const RoPropertyMap& properties)
    {
        mProperties.merge(properties, RoPropertyMap::MergeStrategy::ERROR_ON_EXISTING);
    }

    inline RoPropertyMap getProperties() const
    {
        return mProperties;
    }

private:
    RoPropertyMap mProperties;
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

private:
    RoPropertyMap mProperties;
};

#define roDEFINE_TASK_ARGS(ClassName) struct ClassName : public RoTaskArgsBase<ClassName>

template <typename T>
const T& as_event_args(const RoTaskArgs& args)
{
    const T& event_args_ptr = dynamic_cast<const T&>(args);
    return event_args_ptr;
}

roDEFINE_TASK_ARGS(RoEmptyArgs)
{
    static const RoEmptyArgs INSTANCE;
};