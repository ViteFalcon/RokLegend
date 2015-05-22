////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_RO_GLOB_PATTERN_H
#define ROKLEGEND_RO_GLOB_PATTERN_H

#include "RoPrerequisites.h"

roFORWARD_DECL_PTR(RoGlobPattern);

/**
  * Class that stores 'glob' pattern, which can be used to match strings.
 **/
class RoGlobPattern
{
public: // types
    roFORWARD_DECL_PTR(GlobSubStringMatcher);

public:
    static RoGlobPatternPtr New(const std::string& pattern);

    virtual ~RoGlobPattern();

    bool matches(const std::string& subject) const;

private: // types
    typedef std::list<GlobSubStringMatcherPtr> GlobMatcherList;

private: // Functions
    RoGlobPattern(const GlobMatcherList& matchers);

private: // members
    GlobMatcherList mMatchers;
};

#endif // ROKLEGEND_RO_GLOB_PATTERN_H
