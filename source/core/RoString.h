////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_STRING_H
#define ROKLEGEND_STRING_H

#include "RoTypes.h"
#include "RoSerialization.h"
#include <string>

class RoString
{
public:
    static const RoString EMPTY;

    //! Returns a string that converted from ansi string \a str with EUC-KR encoding
    static RoString FromEucKr(const std::string& str);

     //! Returns a UTF8 string that converted from ansi string \a str with EUC-KR encoding
    static std::string FromEucKrAsUtf8(const std::string& str);

    //! Returns a unicode string that converted from ansi string \a str with current OS specified encoding.
    static RoString FromCurrentEncoding(const std::string& str);

    //! Returns a ansi string that converted from unicode string \a str with current OS specified encoding.
    static std::string ToCurrentEncoding(const RoString& str);

    //! Returns a unicode string that converted from ansi string \a str with \a encodingName encoding.
    static RoString FromEncoding(const std::string& str, const std::string& encodingName);

    //! Returns a ansi string that converted from unicode string \a str with \a encodingName encoding.
    static std::string ToEncoding(const RoString& str, const std::string& encodingName);

    //! Load a text file from \a stream (Byte order mark supported) and returns the text content.
    static RoString FromStream(std::istream& stream);

    //////////////////////////////////////////////////////////////////////////

    typedef std::wstring wstring_type;
    typedef wstring_type::value_type value_type;
    typedef wstring_type::traits_type traits_type;
    typedef wstring_type::allocator_type allocator_type;
    typedef wstring_type::reference reference;
    typedef wstring_type::const_reference const_reference;
    typedef wstring_type::pointer pointer;
    typedef wstring_type::const_pointer const_pointer;
    typedef wstring_type::iterator iterator;
    typedef wstring_type::const_iterator const_iterator;
    typedef wstring_type::reverse_iterator reverse_iterator;
    typedef wstring_type::const_reverse_iterator const_reverse_iterator;
    typedef wstring_type::difference_type difference_type;
    typedef wstring_type::size_type size_type;

    static const size_type npos;

    //////////////////////////////////////////////////////////////////////////

    ~RoString();

    //! default constructor, creates an empty string
    RoString();

    //! duplicate of \a wstr
    RoString(const std::wstring& str);

    //! duplicate of nul-terminated \c wchar_t array
    RoString(const wchar_t* str);

    //! duplicate of \a str, \a length characters long
    RoString(const wchar_t* str, size_type length);

    //! duplicate of \a str (UTF-8 encoding)
    RoString(const std::string& str);

    //! duplicate of nul-terminated C-string \a str (UTF-8 encoding)
    RoString(const char* str);

    //! duplicate of \a str, \a length characters long (UTF-8 encoding)
    RoString(const char* str, size_type length);

    //! \a length copies of \a ch
    RoString(size_type length, wchar_t ch);

    //////////////////////////////////////////////////////////////////////////

    //! Returns the number of code points in the current string
    size_type size() const;

    //! Returns the number of code points in the current string
    size_type length() const;

    //! returns the maximum number of UTF-16 code points that the string can hold
    size_type max_size() const;

    //! sets the capacity of the string to at least \a size code points
    void reserve(size_type size);

    //! changes the size of the string to \a size, filling in any new area with \a val
    void resize(size_type num, const wchar_t& val = 0);

    //! exchanges the elements of the current string with those of \a from
    void swap(RoString& from);

    //! returns \c true if the string has no elements, \c false otherwise
    bool empty() const;

    //! returns a pointer to the first character in the current string
    const wchar_t* c_str() const;

    //! returns the number of elements that the string can hold before it will need to allocate more space
    size_type capacity() const;

    //! deletes all of the elements in the string
    void clear();

    //! returns a substring of the current string, starting at \a index, and \a num characters long.
    /*! If \a num is omitted, it will default to \c UString::npos, and the substr() function will simply return the remainder of the string starting at \a index. */
    RoString substr(size_type index, size_type num = npos) const;

    //! appends \a val to the end of the string
    void push_back(wchar_t val);

    //! appends \a val to the end of the string
    void push_back(char val);

    //! returns a reference to the element in the string at index \c loc
    wchar_t& at(size_type pos);

    //! returns a reference to the element in the string at index \c loc
    const wchar_t& at(size_type pos) const;

    //////////////////////////////////////////////////////////////////////////

    //! returns an iterator to the first element of the string
    iterator begin();

    //! returns an iterator to the first element of the string
    const_iterator begin() const;

    //! returns an iterator just past the end of the string
    iterator end();

    //! returns an iterator just past the end of the string
    const_iterator end() const;

    //! returns a reverse iterator to the last element of the string
    reverse_iterator rbegin();

    //! returns a reverse iterator to the last element of the string
    const_reverse_iterator rbegin() const;

    //! returns a reverse iterator just past the beginning of the string
    reverse_iterator rend();

    //! returns a reverse iterator just past the beginning of the string
    const_reverse_iterator rend() const;

    //////////////////////////////////////////////////////////////////////////

    //! assign \a str to the current string
    RoString& assign(const std::wstring& str);

    //! assign the nul-terminated \a str to the current string
    RoString& assign(const wchar_t* str);

    //! assign the first \a length characters of \a str to the current string
    RoString& assign(const wchar_t* str, size_t length);

    //! assign \a str to the current string
    RoString& assign(const std::string& str);

    //! assign the nul-terminated \a str to the current string
    RoString& assign(const char* str);

    //! assign the first \a length characters of \a str to the current string
    RoString& assign(const char* str, size_t length);

    //! assign \a length copies of \a ch to the current string
    RoString& assign(size_t length, wchar_t ch);

    //////////////////////////////////////////////////////////////////////////

    //! appends \a str on to the end of the current string
    RoString& append(const RoString& str);

    //! appends \a str on to the end of the current string
    RoString& append(const wchar_t* str);

    //! appends \a str on to the end of the current string
    RoString& append(const char* str);

    //! appends a substring of \a str starting at \a index that is \a len characters long on to the end of the current string
    RoString& append(const RoString& str, size_type index, size_type len);

    //! appends the sequence denoted by \a start and \a end on to the end of the current string
    RoString& append(iterator start, iterator end);

    //! appends \a num characters of \a str on to the end of the current string
    RoString& append(const wchar_t* str, size_type num);

    //! appends \a num repetitions of \a ch on to the end of the current string
    RoString& append(size_type num, wchar_t ch);

    //! appends \a num characters of \a str on to the end of the current string
    RoString& append(const char* str, size_type num);

    //! appends \a num repetitions of \a ch on to the end of the current string
    RoString& append(size_type num, char ch);

    //////////////////////////////////////////////////////////////////////////

    //! inserts \a ch before the code point denoted by \a i
    iterator insert(iterator i, const wchar_t& ch);

    //! inserts \a str into the current string, at location \a pos
    RoString& insert(size_type pos, const RoString& str);

    //! inserts \a str into the current string, at location \a pos
    RoString& insert(size_type pos, const wchar_t* str);

    //! inserts a substring of \a str (starting at \a subpos and \a sublen code points long) into the current string, at location \a pos
    RoString& insert(size_type pos, const RoString& str, size_type subpos, size_type sublen);

    //! inserts the code points denoted by \a start and \a end into the current string, before the code point specified by \a i
    void insert(iterator i, iterator start, iterator end);

    //! inserts \a num code points of \a str into the current string, at location \a pos
    RoString& insert(size_type pos, const wchar_t* str, size_type num);

    //! inserts \a num code points of \a str into the current string, at location \a pos
    RoString& insert(size_type pos, const char* str, size_type num);

    //! inserts \a num copies of \a ch into the current string, at location \a pos
    RoString& insert(size_type pos, size_type num, wchar_t ch);

    //! inserts \a num copies of \a ch into the current string, at location \a pos
    RoString& insert(size_type pos, size_type num, char ch);

    //! inserts \a num copies of \a ch into the current string, before the code point denoted by \a i
    void insert(iterator i, size_type num, wchar_t ch);

    //! inserts \a num copies of \a ch into the current string, before the code point denoted by \a i
    void insert(iterator i, size_type num, char ch);

    //////////////////////////////////////////////////////////////////////////

    //! removes the code point pointed to by \a loc, returning an iterator to the next character
    iterator erase(iterator loc);

    //! removes the code points between \a start and \a end (including the one at \a start but not the one at \a end), returning an iterator to the code point after the last code point removed
    iterator erase(iterator start, iterator end);

    //! removes \a num code points from the current string, starting at \a pos
    RoString& erase(size_type pos = 0, size_type num = npos);

    //////////////////////////////////////////////////////////////////////////

    //! replaces up to \a len code points of the current string (starting at \a pos) with \a str
    RoString& replace(size_type pos, size_type len, const RoString& str);

    //! replaces up to \a len code points of the current string (starting at \a pos) with up to \a sublen code points from \a str
    RoString& replace(size_type pos, size_type len, const RoString& str, size_type sublen);

    //! replaces up to \a len code points of the current string (starting at \a pos) with up to \a sublen code points from \a str beginning at \a subpos
    RoString& replace(size_type pos, size_type len, const RoString& str, size_type subpos, size_type sublen);

    //! replaces up to \a len code points in the current string (beginning at \a pos) with \c num copies of \c ch
    RoString& replace(size_type pos, size_type len, size_type num, wchar_t ch);

    //! replaces the code points in the current string from \a start to \a end with \a num copies of \a ch
    RoString& replace(iterator start, iterator end, size_type num, wchar_t ch);

    //////////////////////////////////////////////////////////////////////////

    //! compare \a str to the current string
    int compare(const RoString& str) const;

    //! compare \a str to the current string
    int compare(const wchar_t* str) const;

    //! compare \a str to a substring of the current string, starting at \a pos for \a len characters
    int compare(size_type pos, size_type len, const RoString& str) const;

    //! compare a substring of \a str to a substring of the current string, where \a subpos and \a sublen refer to \a str and \a pos and \a len refer to the current string
    int compare(size_type pos, size_type len, const RoString& str, size_type subpos, size_type sublen) const;

    //! compare a substring of \a str to a substring of the current string, where the substring of \a str begins at zero and is \a num characters long, and the substring of the current string begins at \a pos and is \a len characters long
    int compare(size_type pos, size_type len, const wchar_t* str, size_type num) const;

    //! compare a substring of \a str to a substring of the current string, where the substring of \a str begins at zero and is \a num characters long, and the substring of the current string begins at \a pos and is \a len characters long
    int compare(size_type pos, size_type len, const char* str, size_type num) const;

    //////////////////////////////////////////////////////////////////////////

    //! returns the index of the first occurrence of \a str within the current string, starting at \a index; returns \c UString::npos if nothing is found
    size_type find(const RoString& str, size_type index = 0) const;

    //! returns the index of the first occurrence of \a str within the current string and within \a length code points, starting at \a index; returns \c UString::npos if nothing is found
    size_type find(const char* c_str, size_type index, size_type length) const;

    //! returns the index of the first occurrence of \a str within the current string and within \a length code points, starting at \a index; returns \c UString::npos if nothing is found
    size_type find(const wchar_t* w_str, size_type index, size_type length) const;

    //! returns the index of the first occurrence \a ch within the current string, starting at \a index; returns \c UString::npos if nothing is found
    size_type find(char ch, size_type index = 0) const;

    //! returns the index of the first occurrence \a ch within the current string, starting at \a index; returns \c UString::npos if nothing is found
    size_type find(wchar_t ch, size_type index = 0) const;

    //! returns the location of the first occurrence of \a str in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
    size_type rfind(const RoString& str, size_type index = 0) const;

    //! returns the location of the first occurrence of \a str in the current string, doing a reverse search from \a index, searching at most \a num characters; returns \c UString::npos if nothing is found
    size_type rfind(const char* c_str, size_type index, size_type num) const;

    //! returns the location of the first occurrence of \a str in the current string, doing a reverse search from \a index, searching at most \a num characters; returns \c UString::npos if nothing is found
    size_type rfind(const wchar_t* w_str, size_type index, size_type num) const;

    //! returns the location of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
    size_type rfind(char ch, size_type index = 0) const;

    //! returns the location of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
    size_type rfind(wchar_t ch, size_type index = 0) const;

    //////////////////////////////////////////////////////////////////////////

    //! Returns the index of the first character within the current string that matches \b any character in \a str, beginning the search at \a index and searching at most \a num characters; returns \c UString::npos if nothing is found
    size_type find_first_of(const RoString &str, size_type index = 0, size_type num = npos) const;

    //! returns the index of the first occurrence of \a ch in the current string, starting the search at \a index; returns \c UString::npos if nothing is found
    size_type find_first_of(char ch, size_type index = 0) const;

    //! returns the index of the first occurrence of \a ch in the current string, starting the search at \a index; returns \c UString::npos if nothing is found
    size_type find_first_of(wchar_t ch, size_type index = 0) const;

    //! returns the index of the first character within the current string that does not match any character in \a str, beginning the search at \a index and searching at most \a num characters; returns \c UString::npos if nothing is found
    size_type find_first_not_of(const RoString& str, size_type index = 0, size_type num = npos) const;

    //! returns the index of the first character within the current string that does not match \a ch, starting the search at \a index; returns \c UString::npos if nothing is found
    size_type find_first_not_of(char ch, size_type index = 0) const;

    //! returns the index of the first character within the current string that does not match \a ch, starting the search at \a index; returns \c UString::npos if nothing is found
    size_type find_first_not_of(wchar_t ch, size_type index = 0) const;

    //! returns the index of the first character within the current string that matches any character in \a str, doing a reverse search from \a index and searching at most \a num characters; returns \c UString::npos if nothing is found
    size_type find_last_of(const RoString& str, size_type index = npos, size_type num = npos) const;

    //! returns the index of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
    size_type find_last_of(char ch, size_type index = npos) const;

    //! returns the index of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
    size_type find_last_of(wchar_t ch, size_type index = npos) const;

    //! returns the index of the last character within the current string that does not match any character in \a str, doing a reverse search from \a index; returns \c UString::npos if nothing is found
    size_type find_last_not_of(const RoString& str, size_type index = npos, size_type num = npos) const;

    //! returns the index of the last occurrence of a character that does not match \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
    size_type find_last_not_of(char ch, size_type index = npos) const;

    //! returns the index of the last occurrence of a character that does not match \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
    size_type find_last_not_of(wchar_t ch, size_type index = npos) const;

    //////////////////////////////////////////////////////////////////////////

    bool operator<(const RoString& right) const;

    bool operator<=(const RoString& right) const;

    bool operator>(const RoString& right) const;

    bool operator>=(const RoString& right) const;

    bool operator==(const RoString& right) const;

    bool operator!=(const RoString& right) const;

    RoString& operator=(const RoString& s);

    RoString& operator=(wchar_t ch);

    RoString& operator=(char ch);

    wchar_t& operator[](size_type index);

    const wchar_t& operator[](size_type index) const;

    //////////////////////////////////////////////////////////////////////////

    operator std::wstring() const;

    operator std::string() const;

    //! returns the current string in UTF-8 form within a std::string
    std::string asUTF8() const;

    //! returns the current string in the native form of std::wstring
    std::wstring asWStr() const
    {
        return mString;
    }

    //! returns the current string in the native form of a null-terminated wchar_t array
    const wchar_t* asWStr_c_str() const
    {
        return mString.c_str();
    }

private:
    roS11N_ALLOW_PRIVATE_ACCESS;

    roS11N_SERIALIZE_FUNCTION
    {
        archive & mString;
    }

    std::wstring mString;
};


roINLINE std::ostream& operator << (std::ostream& os, const RoString& s)
{
    return os << s.asUTF8();
}

roINLINE std::wostream& operator << (std::wostream& os, const RoString& s)
{
    return os << s.asWStr();
}

roINLINE RoString operator+(const RoString& s1, const RoString& s2)
{
    return RoString(s1).append(s2);
}

roINLINE RoString operator+(const RoString& s1, wchar_t c)
{
    return RoString(s1).append(1, c);
}

roINLINE RoString operator+( const RoString& s1, char c)
{
    return RoString(s1).append(1, c);
}

roINLINE RoString operator+(wchar_t c, const RoString& s2)
{
    return RoString().append(1, c).append(s2);
}

roINLINE RoString operator+(char c, const RoString& s2)
{
    return RoString().append(1, c).append(s2);
}

roINLINE bool operator == (const char* lhs, const RoString& rhs)
{
    return rhs == lhs;
}

namespace boost
{
    size_t hash_value(const RoString& str);
}

namespace std
{
    template <>
    struct hash < RoString >
    {
        using argument_type = RoString;
        using result_type = hash<std::string>::result_type;

        result_type operator() (argument_type const& value) const
        {
            return std::hash<std::string>()(value.asUTF8());
        }
    };
}

#endif // ROKLEGEND_STRING_H
