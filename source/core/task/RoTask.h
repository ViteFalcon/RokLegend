#pragma once
#include <functional>
#include "RoTaskArgs.h"

using RoTask = std::function < void(const RoTaskArgs&) > ;