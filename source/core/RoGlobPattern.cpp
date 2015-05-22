////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoGlobPattern.h"
#include "RoSharedPtr.h"

#include <boost/algorithm/string.hpp>
//------------------------------------------------------------------------------
#define roGLOB_CHAR '*'
//------------------------------------------------------------------------------
class RoGlobPattern::GlobSubStringMatcher
{
public:
    GlobSubStringMatcher(){}
    virtual ~GlobSubStringMatcher(){}

    virtual bool matches(const std::string& subject) const = 0;
};
//------------------------------------------------------------------------------
class GlobSubStringEquals : public RoGlobPattern::GlobSubStringMatcher
{
public:
    GlobSubStringEquals(const std::string& testString)
        : mTestString(testString)
    {
    }

    virtual bool matches(const std::string& subject) const
    {
        return mTestString == subject;
    }

private:
    const std::string mTestString;
};
//------------------------------------------------------------------------------
class GlobSubStringStartsWith : public RoGlobPattern::GlobSubStringMatcher
{
public:
    GlobSubStringStartsWith(const std::string& startString)
        : mStartString(startString)
    {
    }

    virtual bool matches(const std::string& subject) const
    {
        return boost::algorithm::starts_with(subject, mStartString);
    }

private:
    const std::string mStartString;
};
//------------------------------------------------------------------------------
class GlobSubStringEndsWith : public RoGlobPattern::GlobSubStringMatcher
{
public:
    GlobSubStringEndsWith(const std::string& endString)
        : mEndString(endString)
    {
    }

    virtual bool matches(const std::string& subject) const
    {
        return boost::algorithm::ends_with(subject, mEndString);
    }

private:
    const std::string mEndString;
};
//------------------------------------------------------------------------------
class GlobSubStringHasStringAfterIndex : public RoGlobPattern::GlobSubStringMatcher
{
public:
    GlobSubStringHasStringAfterIndex(const std::string& subString, const size_t startIndex)
        : mSubString(subString)
        , mStartIndex(startIndex)
    {
    }

    virtual bool matches(const std::string& subject) const
    {
        return std::string::npos != subject.find(mSubString, mStartIndex);
    }

private:
    const std::string mSubString;
    const size_t mStartIndex;
};
//------------------------------------------------------------------------------
RoGlobPatternPtr RoGlobPattern::New(const std::string& pattern)
{
    GlobMatcherList matchers;
    if (pattern.empty() || pattern == "*")
    {
        return RoGlobPatternPtr(new RoGlobPattern(matchers));
    }
    std::string::size_type globIndex = 0;
    const std::string::size_type endIndex = pattern.size() - 1;
    size_t globCount = 0;
    do 
    {
        const std::string::size_type nextGlobIndex = pattern.find(roGLOB_CHAR, globIndex);
        if (0 == globIndex && nextGlobIndex == std::string::npos)
        {
            matchers.push_back(std::make_shared<GlobSubStringEquals>(pattern));
        }
        else if (0 == globIndex && nextGlobIndex != 0)
        {
            matchers.push_back(std::make_shared<GlobSubStringStartsWith>(pattern.substr(0, nextGlobIndex)));
        }
        else if (nextGlobIndex == std::string::npos && globIndex < endIndex)
        {
            matchers.push_back(std::make_shared<GlobSubStringEndsWith>(pattern.substr(globIndex)));
        }
        else if (nextGlobIndex != std::string::npos && globIndex != nextGlobIndex)
        {
            matchers.push_back(std::make_shared<GlobSubStringHasStringAfterIndex>(pattern.substr(globIndex, nextGlobIndex-globIndex), globIndex-globCount));
        }

        globIndex = (std::string::npos == nextGlobIndex || endIndex <= globIndex) ? std::string::npos : nextGlobIndex + 1;
        ++globCount;

    } while (std::string::npos != globIndex);
    return RoGlobPatternPtr(new RoGlobPattern(matchers));
}
//------------------------------------------------------------------------------
RoGlobPattern::RoGlobPattern(const GlobMatcherList& matchers)
    : mMatchers(matchers)
{
}
//------------------------------------------------------------------------------
RoGlobPattern::~RoGlobPattern()
{
}
//------------------------------------------------------------------------------
bool RoGlobPattern::matches(const std::string& subject) const
{
    if (mMatchers.empty())
    {
        return true;
    }
    bool subjectMatches = true;
    for (auto matcher : mMatchers)
    {
        subjectMatches = matcher->matches(subject);
        if (!subjectMatches)
        {
            break;
        }
    }
    return subjectMatches;
}