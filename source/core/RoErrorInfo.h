#pragma once
#ifndef ROKLEGEND_ERROR_INFO_H
#define ROKLEGEND_ERROR_INFO_H

#include "RoPrerequisites.h"
#include <boost/exception/error_info.hpp>

#define roERROR_INFO_TAGNAME(name) roCOMBINE(RoErrorTag,name)

#define roERROR_INFO_NAME(name) roCOMBINE(RoErrorInfo,name)

#define roERROR_INFO(name,type) struct roERROR_INFO_TAGNAME(name)\
    {\
        roERROR_INFO_TAGNAME(name)(const type& val)\
            : value(val)\
        {\
        }\
        const type value;\
    };\
    typedef boost::error_info<roERROR_INFO_TAGNAME(name), type> roERROR_INFO_NAME(name)

// RoErrorInfoFileName: Specifies name of the file relevant to the error.
roERROR_INFO(FileName, RoString);

roERROR_INFO(Directory, RoString);

// RoErrorInfoDetail: Specifies details about the error.
roERROR_INFO(Detail, RoString);

roERROR_INFO(Hint, RoString);

roERROR_INFO(ItemName, RoString);

struct roERROR_INFO_TAGNAME(InnerException)
{
    roERROR_INFO_TAGNAME(InnerException)(const std::exception& val)
        : value(val)
    {
    }

    const std::exception& value;
};
typedef boost::error_info<roERROR_INFO_TAGNAME(InnerException), std::exception> roERROR_INFO_NAME(InnerException);

#endif // ROKLEGEND_ERROR_INFO_H
