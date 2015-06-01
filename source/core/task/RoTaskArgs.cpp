#include "RoTaskArgs.h"

const RoEmptyArgs RoEmptyArgs::INSTANCE{};

RoTaskArgsPtr RoTaskArgs::clone() const
{
    return clone(-1);
}

void RoTaskArgs::set(const RoHashString& name, const RoVariant& val)
{
    mProperties.set(name, val);
}
