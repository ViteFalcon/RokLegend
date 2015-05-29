#pragma once
#include <core/RoPrerequisites.h>

class RoDataInfo
{
public: // static
    static RoDataInfo& Get();

public:
    RoDataInfo() = default;
    virtual ~RoDataInfo() = default;

    virtual RoOptionalString getValue(const RoString& key) const = 0;

    RoString getValue(const RoString& key, const RoString& defaultValue);
};