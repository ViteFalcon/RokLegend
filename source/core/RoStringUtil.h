////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROGRF_STRING_UTIL_H
#define ROGRF_STRING_UTIL_H

#include "RoPrerequisites.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/detail/case_conv.hpp>
#include <boost/scoped_array.hpp>

#include <cstdarg>

/**
  * Utility class for strings.
 **/
class RoStringUtil
{
public:
    static const RoString Empty;

    roINLINE static RoString ToLower(const RoString& string)
    {
        return boost::to_lower_copy(string.asWStr());
    }

    roINLINE static StringArray Split(const RoString& string, const RoString& delimiters)
    {
        StringArray splits;
        boost::split(splits, string.asWStr(), boost::is_any_of(delimiters.asWStr()));
        return splits;
    }

    static RoString Format(const char* format, ...)
    {
        va_list argList;
        va_start(argList, format);
        RoString string = VFormat(format, argList);
        va_end(argList);
        return string;
    }

    static RoString Format(const wchar_t* format, ...)
    {
        va_list argList;
        va_start(argList, format);
        RoString string = VFormat(format, argList);
        va_end(argList);
        return string;
    }

    roINLINE static RoString VFormat(const wchar_t* format, va_list argList)
    {
#ifdef _WIN32
        int32 size = _vscwprintf(format, argList) + 1;
#else
        int32 size = vswprintf(0, 0, format, argList) + 1;
#endif
        if (size > 0)
        {
            boost::scoped_array<wchar_t> formattedString(new wchar_t[size]);
            vswprintf(formattedString.get(), size, format, argList);
            return formattedString.get();
        }
        return RoStringUtil::Empty;
    }

    roINLINE static RoString VFormat(const char* format, va_list argList)
    {
#ifdef _WIN32
        int32 size = _vscprintf(format, argList) + 1;
#else
        int32 size = vsnprintf(0, 0, format, argList) + 1;
#endif
        if (size > 0)
        {
            boost::scoped_array<char> formattedString(new char[size]);
            vsnprintf(formattedString.get(), size, format, argList);
            return formattedString.get();
        }
        return RoStringUtil::Empty;
    }

    roINLINE static RoOptionalBool ParseBool(const RoString& val)
    {
        RoOptionalBool result;
        RoString valInLowerCase = ToLower(val);
        if (valInLowerCase == L"true" || valInLowerCase == L"yes" || valInLowerCase == L"on" || valInLowerCase == L"1")
        {
            result = true;
        }
        else if (valInLowerCase == L"false" || valInLowerCase == L"no" || valInLowerCase == L"off"  || valInLowerCase == L"0")
        {
            result = false;
        }
        return result;
    }

    roINLINE static void Trim(RoString& stringToTrim)
    {
        std::wstring wstrString = stringToTrim.asWStr();
        boost::trim(wstrString);
        stringToTrim = wstrString;
    }

    roINLINE static void Trim(std::string& stringToTrim)
    {
        boost::trim(stringToTrim);
    }

    roINLINE static RoString TrimCopy(const RoString& stringToTrim)
    {
        RoString result(stringToTrim);
        Trim(result);
        return result;
    }

    roINLINE static bool IsBlank(const RoString& str)
    {
        return IsBlank(str, std::locale());
    }

    roINLINE static bool IsBlank(const RoString& str, const std::locale& locale)
    {
        if (str.empty())
        {
            return true;
        }
        return boost::algorithm::all(str.asWStr(), boost::is_space(locale));
    }

private:
    RoStringUtil();
    virtual ~RoStringUtil();
};

#endif // ROGRF_STRING_UTIL_H
