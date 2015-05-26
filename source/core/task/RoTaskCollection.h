#pragma once
#include "../RoPrerequisites.h"
#include "../RoVector.h"

#include "RoTask.h"

class RoTaskCollection
{
public:
    RoTaskCollection() = default;
    ~RoTaskCollection();

    void add(const RoString& name, const RoTask& task);

    void clear();

private:
    using TaskNames = RoVector < std::string > ;
    TaskNames mTaskNames;
};