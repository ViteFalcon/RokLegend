////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_STRING_BUILDER_H
#define ROKLEGEND_STRING_BUILDER_H

#include "RoPrerequisites.h"
#include <sstream>

/**
  * Brief description about RoStringBuilder.
 **/
class RoStringBuilder
{
public:
    static RoString Format(const char* format, ...);
    static RoString Format(const wchar_t* format, ...);

    typedef std::basic_ios<wchar_t, std::char_traits<wchar_t> >& (*BasicIosStreamModifier)(std::basic_ios<wchar_t, std::char_traits<wchar_t> >&);
    typedef std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& (*BasicOstreamModifier)(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >&);

    RoStringBuilder(size_t indentWidth = 0);
    virtual ~RoStringBuilder();

    RoStringBuilder& append(const RoString& str);
    RoStringBuilder& appendLine(const RoString& str);

	template <typename T>
	RoStringBuilder& append(const T& val)
	{
		std::stringstream stream;
		stream << val;
		return appendStr(RoString(stream.str()));
	}

	template <typename T>
	RoStringBuilder& appendLine(const T& val)
	{
		std::stringstream stream;
		stream << val;
		return appendStrLine(RoString(stream.str()));
	}

    RoStringBuilder& append(const char* format, ...);
    RoStringBuilder& appendLine(const char* format, ...);

    RoStringBuilder& append(const wchar_t* format, ...);
    RoStringBuilder& appendLine(const wchar_t* format, ...);

    RoStringBuilder& appendStr(const RoString& format, ...);
    RoStringBuilder& appendStrLine(const RoString& format, ...);

    inline RoStringBuilder& increaseIndentation()
    {
        return setIndentation(mIndentationSize + 1);
    }

    inline RoStringBuilder& decreaseIndentation()
    {
        return setIndentation(mIndentationSize - 1);
    }

    inline RoStringBuilder& setIndentation(size_t indentationSize)
    {
        if (indentationSize < 0)
        {
            indentationSize = 0;
        }
        RoString spaces;
        if (indentationSize)
        {
            spaces = RoString(indentationSize*4, ' ');
        }
        RoString newLine = RoString(indentationSize*4+1, ' ');
        newLine[0] = '\n';
        mNewLine.swap(newLine);
        mSpaces.swap(spaces);
        mIndentationSize = indentationSize;
        return *this;
    }

    inline size_t getIndentation() const
    {
        return mIndentationSize;
    }

    inline RoString toString() const
    {
        return mStream.str();
    }

    RoStringBuilder& applyStreamModifier(BasicOstreamModifier modifier)
    {
        modifier(mStream);
        return *this;
    }

    inline RoStringBuilder& operator << (BasicOstreamModifier modifier)
    {
        return applyStreamModifier(modifier);
    }

    inline operator RoString() const
    {
        return mStream.str();
    }

    friend std::ostream& operator << (std::ostream& stream, const RoStringBuilder& stringBuilder)
    {
        return stream << stringBuilder.toString();
    }

    friend std::wostream& operator << (std::wostream& stream, const RoStringBuilder& stringBuilder)
    {
        return stream << stringBuilder.toString();
    }

private:
    RoString            mNewLine;
    RoString            mSpaces;
    size_t              mIndentationSize;
    std::wstringstream  mStream;
};


template <typename T>
inline RoStringBuilder& operator << (RoStringBuilder& builder, const T& val)
{
    std::wstringstream stream;
    stream << val;
    return builder.append(stream.str());
}

#endif // ROKLEGEND_STRING_BUILDER_H
