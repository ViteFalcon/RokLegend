#pragma once
#ifndef RO_SHARED_PTR_H
#define RO_SHARED_PTR_H

#include "RoDefines.h"
#include <memory>

template <typename T>
using RoSharedPtr = std::shared_ptr<T>;

#define roDEFINE_PTR(classname) using roCOMBINE(classname,Ptr) = RoSharedPtr<classname>
#define roFORWARD_DECL_PTR(classname) class classname; roDEFINE_PTR(classname)

#endif // RO_SHARED_PTR_H
