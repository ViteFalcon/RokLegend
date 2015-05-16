////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoStringBuilder.h"
#include "RoStringUtil.h"

#include <cstdarg>

RoStringBuilder::RoStringBuilder(size_t indentWidth)
{
    setIndentation(indentWidth);
}

RoStringBuilder::~RoStringBuilder()
{
}

RoStringBuilder& RoStringBuilder::append(const RoString& str)
{
    if (mStream.str().empty() || '\n' == *(--mStream.str().end()))
    {
        mStream<<mSpaces;
    }
    StringArray splits = RoStringUtil::Split(str, "\n");
    if (splits.empty())
    {
        return *this;
    } 
    for (size_t i = 0; i < splits.size()-1; ++i)
    {
        mStream<<splits[i]<<mNewLine;
    }
    mStream<<splits.back();
    return *this;
}

RoStringBuilder& RoStringBuilder::appendLine(const RoString& str)
{
    append(str);
    mStream<<std::endl;
    return *this;
}

RoStringBuilder& RoStringBuilder::append(const char* format, ...)
{
    va_list argList;
    va_start(argList, format);
    append(RoStringUtil::VFormat(format, argList));
    va_end(argList);
    return *this;
}

RoStringBuilder& RoStringBuilder::appendLine(const char* format, ...)
{
    va_list argList;
    va_start(argList, format);
    appendLine(RoStringUtil::VFormat(format, argList));
    va_end(argList);
    return *this;
}

RoStringBuilder& RoStringBuilder::append(const wchar_t* format, ...)
{
    va_list argList;
    va_start(argList, format);
    append(RoStringUtil::VFormat(format, argList));
    va_end(argList);
    return *this;
}

RoStringBuilder& RoStringBuilder::appendLine(const wchar_t* format, ...)
{
    va_list argList;
    va_start(argList, format);
    appendLine(RoStringUtil::VFormat(format, argList));
    va_end(argList);
    return *this;
}

RoStringBuilder& RoStringBuilder::appendStr(const RoString& format, ...)
{
    va_list argList;
    va_start(argList, format);
    append(RoStringUtil::VFormat(format.c_str(), argList));
    va_end(argList);
    return *this;
}

RoStringBuilder& RoStringBuilder::appendStrLine(const RoString& format, ...)
{
    va_list argList;
    va_start(argList, format);
    appendLine(RoStringUtil::VFormat(format.c_str(), argList));
    va_end(argList);
    return *this;
}
