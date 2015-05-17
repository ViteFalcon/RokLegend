////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_HASH_STRING_H
#define ROKLEGEND_HASH_STRING_H

#include "RoDefines.h"
#include "RoString.h"

#define roHASHSTRING_STORE_STRING 1

#define roFNV_OFFSET  2166136261u
#define roFNV_PRIME   16777619u

template <size_t N, size_t I>
struct RoFnvHash
{
    roINLINE static size_t Hash(const char (&str)[N])
    {
        return (RoFnvHash<N, I-1>::Hash(str) ^ str[I-1])*roFNV_PRIME;
    }
};

template <size_t N>
struct RoFnvHash<N, 0>
{
    roINLINE static size_t Hash(const char (&str)[N])
    {
        (void)str;
        return roFNV_OFFSET;
    }
};

/**
  * Brief description about RoHashString.
 **/
class RoHashString
{
public:
    inline static RoHashString FromString(const RoString& string)
    {
        auto stdString = string.asUTF8();
        return RoHashString{ stdString.c_str() };
    }

    struct ConstCharWrapper
    {
        roINLINE ConstCharWrapper(const char* str)
            :str(str){}

        const char* str;
    };

    RoHashString()
        : mHash(RoFnvHash<1,0>::Hash(""))
    {
    }

    template <size_t N>
    roINLINE RoHashString(const char (&str)[N])
        : mHash(RoFnvHash<N,N-1>::Hash(str))
#if roHASHSTRING_STORE_STRING
        , mString(str)
#endif
    {
    }

    // Run-time hashing
    explicit RoHashString(ConstCharWrapper str);

    ~RoHashString();

#if roHASHSTRING_STORE_STRING
    roINLINE const std::string& str() const
    {
        return mString;
    }
#endif // _DEBUG

    roINLINE bool empty() const
    {
        return roFNV_OFFSET == mHash;
    }

    roINLINE void swap(RoHashString& other)
    {
        std::swap(mHash, other.mHash);
#if roHASHSTRING_STORE_STRING
        mString.swap(other.mString);
#endif //_DEBUG
    }

    roINLINE bool equals(const RoHashString& rhs) const
    {
        return mHash == rhs.mHash;
    }

    roINLINE bool operator == (const RoHashString& rhs) const
    {
        return equals(rhs);
    }

    roINLINE bool operator != (const RoHashString& rhs) const
    {
        return !equals(rhs);
    }

    roINLINE RoHashString& operator = (const RoHashString& rhs)
    {
        RoHashString(rhs).swap(*this);
        return *this;
    }

    roINLINE size_t hash() const
    {
        return mHash;
    }

    roINLINE bool operator < (const RoHashString& rhs) const
    {
        return mHash < rhs.mHash;
    }

    friend std::ostream& operator << (std::ostream& stream, const RoHashString& str)
    {
#if roHASHSTRING_STORE_STRING
        return stream << str.mString;
#else
        return stream << "#" << str.mHash;
#endif // _DEBUG
    }

    friend std::wostream& operator << (std::wostream& stream, const RoHashString& str)
    {
#if roHASHSTRING_STORE_STRING
        RoString wideString(str.mString);
        return stream << wideString;
#else
        return stream << "#" << str.mHash;
#endif // _DEBUG
    }

private:
    size_t mHash;
#if roHASHSTRING_STORE_STRING
    std::string mString;
#endif // _DEBUG
};

#define _H(str) RoHashString(str)

namespace boost{
    roINLINE size_t hash_value(const RoHashString& str)
    {
        return str.hash();
    }
}

namespace std {
    template <>
    struct hash<RoHashString>
    {
        inline size_t operator()(const RoHashString& key)
        {
            return key.hash();
        }
    };
}

#endif // ROKLEGEND_HASH_STRING_H
