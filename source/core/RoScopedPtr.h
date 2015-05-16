////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_SCOPED_PTR_H
#define ROKLEGEND_SCOPED_PTR_H

#include "RoPrerequisites.h"
#include <boost/scoped_array.hpp>

template <typename T>
using RoScopedPtr = unique_ptr<T>;

template <typename T>
using RoScopedArray = boost::scoped_array<T>;

#define roDEFINE_SCOPED_PTR(classname) typedef RoScopedPtr<classname> roCOMBINE(classname, Ptr)
#define roFORWARD_DECL_SCOPED_PTR(classname) class classname; roDEFINE_SCOPED_PTR(classname);

#endif // ROKLEGEND_SCOPED_PTR_H
