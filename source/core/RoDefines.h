#pragma once
#ifndef ROKLEGEND_DEFINES_H
#define ROKLEGEND_DEFINES_H

#include "RoPlatform.h"

#define _CRT_RAND_S

#if roCOMPILER_IS_MSVC
#   pragma warning(disable:4251)
#   pragma warning(disable:4275)
#   pragma warning(disable:4503)
#   define roDEPRECATED(func) __declspec(deprecated) func
#elif roCOMPILER_IS_GNUC
#   define roDEPRECATED(func) func __attribute__ ((deprecated))
#else
#   message("WARNING: This compiler doesn't have deprecation functionality.")
#   define roDEPRECATED(func) func
#endif // roCOMPILER_IS_MSVC

#if roPLATFORM_IS_WIN32

#   define roFILENAME (strrchr(__FILE__,'\\') \
    ? strrchr(__FILE__,'\\')+1 \
    : __FILE__ \
    )
#   define roINLINE __forceinline
#else
#   define roFILENAME (strrchr(__FILE__,'/') \
    ? strrchr(__FILE__,'/')+1 \
    : __FILE__ \
    )
#   if roCOMPILER_IS_GNUC
#       define roINLINE inline __attribute__((always_inline))
#   else
#       define roINLINE inline
#   endif
#endif

#define roSTRINGIFY(a) # a
#define roCOMBINE(a,b) a ## b
#define roWHERE __FUNCTION__<<"["<<roFILENAME<<" @ Line: "<<__LINE__<<"]"

#if __cplusplus > 199711L
#   define roCPP_11 1
#   define roMOVREF &&
#else
#   define roCPP_11 0
#   define roMOVREF &
#endif // __cplusplus

#define roMOVREF_TYPE(type) type roMOVREF

#define roCLIENT_VERSION 114

#if defined(TBB_PREVIEW_MEMORY_POOL) && TBB_PREVIEW_MEMORY_POOL
#   define roUSE_TBB_MEMORY_POOL 1
#else
#   define roUSE_TBB_MEMORY_POOL 0
#endif

#if defined(_DEBUG)
#   define roDEBUG_MODE 1
#else
#   define roDEBUG_MODE 0
#endif

#include <cassert>

#define roDEBUG_BREAK_IF(condition) assert(condition)

#endif // ROKLEGEND_DEFINES_H
