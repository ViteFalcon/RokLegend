////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_PLATFORM_H
#define ROKLEGEND_PLATFORM_H

#include <string>

#define roPLATFORM_WIN32    1
#define roPLATFORM_LINUX    2
#define roPLATFORM_APPLE    3

#define roCOMPILER_MSVC     1
#define roCOMPILER_GNUC     2

#define roARCHITECTURE_32   1
#define roARCHITECTURE_64   2

// Find the platform
#if defined(__WIN32__) || defined(_WIN32)
#   define roPLATFORM roPLATFORM_WIN32
#elif defined(__APPLE_CC__)
#   define roPLATFORM roPLATFORM_APPLE
#else
#   define roPLATFORM roPLATFORM_LINUX
#endif

// Find the compiler
#if defined(_MSC_VER)
#   define roCOMPILER roCOMPILER_MSVC
#   define roCOMPILER_VERSION _MSC_VER
#elif defined(__GNUC__)
#   define roCOMPILER roCOMPILER_GNUC
#   define roCOMPILER_VERSION (((__GNUC__)*100) + \
    (__GNUC_MINOR__*10) + \
    __GNUC_PATCHLEVEL__)
#else
#   pragma error "Unknown compiler! Abort! Abort!"
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define roARCHITECTURE roARCHITECTURE_64
#else
#   define roARCHITECTURE roARCHITECTURE_32
#endif // roARCHITECTURE

#define roPLATFORM_IS_WIN32 roPLATFORM == roPLATFORM_WIN32
#define roPLATFORM_IS_LINUX roPLATFORM == roPLATFORM_LINUX
#define roPLATFORM_IS_APPLE roPLATFORM == roPLATFORM_APPLE

#define roCOMPILER_IS_MSVC  roCOMPILER == roCOMPILER_MSVC
#define roCOMPILER_IS_GNUC  roCOMPILER == roCOMPILER_GNUC

#define roARCHITECTURE_IS_32 roARCHITECTURE == roARCHITECTURE_32
#define roARCHITECTURE_IS_64 roARCHITECTURE == roARCHITECTURE_64

#if roPLATFORM_IS_WIN32
#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN
//  Including SDKDDKVer.h defines the highest available Windows platform.
//  If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
//  set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#   include <SDKDDKVer.h>
#   include <Windows.h>
#endif // roPLATFORM_IS_WIN32

extern const std::string roPlatform;
extern const std::string roCompiler;
extern const std::string roCompilerVersion;

#endif // ROKLEGEND_PLATFORM_H
