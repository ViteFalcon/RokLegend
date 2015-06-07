////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoString.h"
#include "RoPlatform.h"
#include "RoScopedPtr.h"
#include <boost/locale.hpp>
#include <boost/lexical_cast.hpp>

const RoString RoString::EMPTY;

RoString::~RoString()
{

}

RoString::RoString()
{

}

RoString::RoString(const std::wstring& str)
{
    assign(str);
}

RoString::RoString(const wchar_t* str)
{
    assign(str);
}

RoString::RoString(const wchar_t* str, size_type length)
{
    assign(str, length);
}

RoString::RoString(const std::string& str)
{
    assign(str);
}

RoString::RoString(const char* str)
{
    assign(str);
}

RoString::RoString(const char* str, size_type length)
{
    assign(str, length);
}

RoString::RoString(size_type length, wchar_t ch)
{
    assign(length, ch);
}

RoString::size_type RoString::size() const
{
    return mString.size();
}

RoString::size_type RoString::length() const
{
    return mString.length();
}

RoString::size_type RoString::max_size() const
{
    return mString.max_size();
}

void RoString::reserve(size_type size)
{
    mString.reserve(size);
}

void RoString::resize(size_type num, const wchar_t& val /*= 0*/)
{
    mString.resize(num, val);
}

void RoString::swap(RoString& from)
{
    mString.swap(from.mString);
}

bool RoString::empty() const
{
    return mString.empty();
}

const wchar_t* RoString::c_str() const
{
     return mString.c_str();
}

RoString::size_type RoString::capacity() const
{
    return mString.capacity();
}

void RoString::clear()
{
    mString.clear();
}

RoString RoString::substr(size_type index, size_type num /*= npos*/) const
{
    return mString.substr(index, num);
}

void RoString::push_back(wchar_t val)
{
    mString.push_back(val);
}

void RoString::push_back(char val)
{
    mString.push_back(val);
}

wchar_t& RoString::at(size_type pos)
{
    return mString.at(pos);
}

const wchar_t& RoString::at(size_type pos) const
{
    return mString.at(pos);
}

RoString::iterator RoString::begin()
{
    return mString.begin();
}

RoString::const_iterator RoString::begin() const
{
    return mString.begin();
}

RoString::iterator RoString::end()
{
    return mString.end();
}

RoString::const_iterator RoString::end() const
{
    return mString.end();
}

RoString::reverse_iterator RoString::rbegin()
{
    return mString.rbegin();
}

RoString::const_reverse_iterator RoString::rbegin() const
{
    return mString.rbegin();
}

RoString::reverse_iterator RoString::rend()
{
    return mString.rend();
}

RoString::const_reverse_iterator RoString::rend() const
{
    return mString.rend();
}

RoString& RoString::assign(const std::wstring& str)
{
    mString.assign(str);
    return *this;
}

RoString& RoString::assign(const wchar_t* str)
{
    mString.assign(str);
    return *this;
}

RoString& RoString::assign(const wchar_t* str, size_t length)
{
    mString.assign(str, length);
    return *this;
}

RoString& RoString::assign(const std::string& str)
{
    mString.assign(boost::locale::conv::utf_to_utf<wchar_t>(str));
    return *this;
}

RoString& RoString::assign(const char* str)
{
    mString.assign(boost::locale::conv::utf_to_utf<wchar_t>(str));
    return *this;
}

RoString& RoString::assign(const char* str, size_t length)
{
    mString.assign(boost::locale::conv::utf_to_utf<wchar_t>(str, str+length));
    return *this;
}

RoString& RoString::assign(size_t length, wchar_t ch)
{
    mString.assign(length, ch);
    return *this;
}

RoString& RoString::append(const RoString& str)
{
    mString.append(str.mString);
    return *this;
}

RoString& RoString::append(const wchar_t* str)
{
    mString.append(str);
    return *this;
}

RoString& RoString::append(const char* str)
{
    mString.append(boost::locale::conv::utf_to_utf<wchar_t>(str));
    return *this;
}

RoString& RoString::append(const RoString& str, size_type index, size_type len)
{
    mString.append(str.mString, index, len);
    return *this;
}

RoString& RoString::append(iterator start, iterator end)
{
    mString.append(start, end);
    return *this;
}

RoString& RoString::append(const wchar_t* str, size_type num)
{
    mString.append(str, num);
    return *this;
}

RoString& RoString::append(size_type num, wchar_t ch)
{
    mString.append(num, ch);
    return *this;
}

RoString& RoString::append(const char* str, size_type num)
{
    mString.append(boost::locale::conv::utf_to_utf<wchar_t>(str, str+num));
    return *this;
}

RoString& RoString::append(size_type num, char ch)
{
    mString.append(num, ch);
    return *this;
}

RoString::iterator RoString::insert(iterator i, const wchar_t& ch)
{
    return mString.insert(i, ch);
}

RoString& RoString::insert(size_type pos, const RoString& str)
{
    mString.insert(pos, str.mString);
    return *this;
}

RoString& RoString::insert(size_type pos, const wchar_t* str)
{
    mString.insert(pos, str);
    return *this;
}

RoString& RoString::insert(size_type pos, const RoString& str, size_type subpos, size_type sublen)
{
    mString.insert(pos, str.mString, subpos, sublen);
    return *this;
}

void RoString::insert(iterator i, iterator start, iterator end)
{
    mString.insert(i, start, end);
}

RoString& RoString::insert(size_type pos, const wchar_t* str, size_type num)
{
    mString.insert(pos, str, num);
    return *this;
}

RoString& RoString::insert(size_type pos, const char* str, size_type num)
{
    mString.insert(pos, boost::locale::conv::utf_to_utf<wchar_t>(str).c_str(), num);
    return *this;
}

RoString& RoString::insert(size_type pos, size_type num, wchar_t ch)
{
    mString.insert(pos, num, ch);
    return *this;
}

RoString& RoString::insert(size_type pos, size_type num, char ch)
{
    mString.insert(pos, num, ch);
    return *this;
}

void RoString::insert(iterator i, size_type num, wchar_t ch)
{
    mString.insert(i, num, ch);
}

void RoString::insert(iterator i, size_type num, char ch)
{
    mString.insert(i, num, ch);
}

RoString::iterator RoString::erase(iterator loc)
{
    return mString.erase(loc);
}

RoString::iterator RoString::erase(iterator start, iterator end)
{
    return mString.erase(start, end);
}

RoString& RoString::erase(size_type pos /*= 0*/, size_type num /*= npos*/)
{
    mString.erase(pos, num);
    return *this;
}

RoString& RoString::replace(size_type pos, size_type len, const RoString& str)
{
    mString.replace(pos, len, str.mString);
    return *this;
}

RoString& RoString::replace(size_type pos, size_type len, const RoString& str, size_type sublen)
{
    mString.replace(pos, len, str.mString.c_str(), sublen);
    return *this;
}

RoString& RoString::replace(size_type pos, size_type len, const RoString& str, size_type subpos, size_type sublen)
{
    mString.replace(pos, len, str.mString, subpos, sublen);
    return *this;
}

RoString& RoString::replace(size_type pos, size_type len, size_type num, wchar_t ch)
{
    mString.replace(pos, len, num, ch);
    return *this;
}

RoString& RoString::replace(iterator start, iterator end, size_type num, wchar_t ch)
{
    mString.replace(start, end ,num, ch);
    return *this;
}

int RoString::compare(const RoString& str) const
{
    return mString.compare(str.mString);
}

int RoString::compare(const wchar_t* str) const
{
    return mString.compare(str);
}

int RoString::compare(size_type pos, size_type len, const RoString& str) const
{
    return mString.compare(pos, len, str.mString);
}

int RoString::compare(size_type pos, size_type len, const RoString& str, size_type subpos, size_type sublen) const
{
    return mString.compare(pos, len, str.mString, subpos, sublen);
}

int RoString::compare(size_type pos, size_type len, const wchar_t* str, size_type num) const
{
    return mString.compare(pos, len, str, num);
}

int RoString::compare(size_type pos, size_type len, const char* str, size_type num) const
{
    return mString.compare(pos, len, boost::locale::conv::utf_to_utf<wchar_t>(str).c_str(), num);
}

RoString::size_type RoString::find(const RoString& str, size_type index /*= 0*/) const
{
    return mString.find(str, index);
}

RoString::size_type RoString::find(const char* str, size_type index, size_type length) const
{
    return mString.find(boost::locale::conv::utf_to_utf<wchar_t>(str).c_str(), index, length);
}

RoString::size_type RoString::find(const wchar_t* str, size_type index, size_type length) const
{
    return mString.find(str, index, length);
}

RoString::size_type RoString::find(char ch, size_type index /*= 0*/) const
{
    return mString.find(ch, index);
}

RoString::size_type RoString::find(wchar_t ch, size_type index /*= 0*/) const
{
    return mString.find(ch, index);
}

RoString::size_type RoString::rfind(const RoString& str, size_type index /*= 0*/) const
{
    return mString.rfind(str, index);
}

RoString::size_type RoString::rfind(const wchar_t* str, size_type index, size_type num) const
{
    return mString.rfind(str, index, num);
}

RoString::size_type RoString::rfind(const char* str, size_type index, size_type num) const
{
    return mString.rfind(boost::locale::conv::utf_to_utf<wchar_t>(str).c_str(), index, num);
}

RoString::size_type RoString::rfind(char ch, size_type index /*= 0*/) const
{
    return mString.rfind(ch, index);
}

RoString::size_type RoString::rfind(wchar_t ch, size_type index) const
{
    return mString.rfind(ch, index);
}

RoString::size_type RoString::find_first_of(const RoString &str, size_type index /*= 0*/, size_type num /*= npos*/) const
{
    return mString.find_first_of(str.mString.c_str(), index, num);
}

RoString::size_type RoString::find_first_of(wchar_t ch, size_type index /*= 0*/) const
{
    return mString.find_first_of(ch, index);
}

RoString::size_type RoString::find_first_of(char ch, size_type index /*= 0*/) const
{
    return mString.find_first_of(ch, index);
}

RoString::size_type RoString::find_first_not_of(const RoString& str, size_type index /*= 0*/, size_type num /*= npos*/) const
{
    return mString.find_first_not_of(str.mString.c_str(), index, num);
}

RoString::size_type RoString::find_first_not_of(wchar_t ch, size_type index /*= 0*/) const
{
    return mString.find_first_not_of(ch, index);
}

RoString::size_type RoString::find_first_not_of(char ch, size_type index /*= 0*/) const
{
    return mString.find_first_not_of(ch, index);
}

RoString::size_type RoString::find_last_of(const RoString& str, size_type index /*= npos*/, size_type num /*= npos*/) const
{
    return mString.find_last_of(str.mString.c_str(), index, num);
}

RoString::size_type RoString::find_last_of(wchar_t ch, size_type index /*= npos*/) const
{
    return mString.find_last_of(ch, index);
}

RoString::size_type RoString::find_last_of(char ch, size_type index /*= npos*/) const
{
    return mString.find_last_of(ch, index);
}

RoString::size_type RoString::find_last_not_of(const RoString& str, size_type index /*= npos*/, size_type num /*= npos*/) const
{
    return mString.find_last_not_of(str.mString.c_str(), index, num);
}

RoString::size_type RoString::find_last_not_of(wchar_t ch, size_type index /*= npos*/) const
{
    return mString.find_last_not_of(ch, index);
}

RoString::size_type RoString::find_last_not_of(char ch, size_type index /*= npos*/) const
{
    return mString.find_last_not_of(ch, index);
}

bool RoString::operator<(const RoString& right) const
{
    return compare(right) < 0;
}

bool RoString::operator<=(const RoString& right) const
{
    return compare(right) <= 0;
}

bool RoString::operator>(const RoString& right) const
{
    return compare(right) > 0;
}

bool RoString::operator>=(const RoString& right) const
{
    return compare(right) >= 0;
}

bool RoString::operator==(const RoString& right) const
{
    return compare(right) == 0;
}

bool RoString::operator!=(const RoString& right) const
{
    return !operator==(right);
}

RoString& RoString::operator=(const RoString& s)
{
    mString.operator =(s.mString);
    return *this;
}

RoString& RoString::operator=(wchar_t ch)
{
    mString.operator =(ch);
    return *this;
}

RoString& RoString::operator=(char ch)
{
    mString.operator =(ch);
    return *this;
}

wchar_t& RoString::operator[](size_type index)
{
    return mString.operator [](index);
}

const wchar_t& RoString::operator[](size_type index) const
{
    return mString.operator [](index);
}

RoString::operator std::wstring() const
{
    return mString;
}

RoString::operator std::string() const
{
    return asUTF8();
}

#include <locale>
#include <codecvt>

std::string RoString::asUTF8() const
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    //std::string utf8 = boost::locale::conv::utf_to_utf<char>(mString);
    std::string utf8 = conv.to_bytes(mString.c_str());
    return utf8;
}

const RoString::size_type RoString::npos = RoString::wstring_type::npos;

roINLINE const std::string& getCurrentLocale()
{
    static boost::optional<std::string> staticCurrentLocale;

    if (!staticCurrentLocale.is_initialized())
    {
#ifdef _WIN32
        // Corresponded and copied from boost\libs\locale\src\encoding\wconv_codepage.ipp
        struct WindowsEncoding
        {
            char const *name;
            int32 codepage;
        } allWindowsEncodings[] =
        {
            { "big5",       950 },
            { "cp1250",     1250 },
            { "cp1251",     1251 },
            { "cp1252",     1252 },
            { "cp1253",     1253 },
            { "cp1254",     1254 },
            { "cp1255",     1255 },
            { "cp1256",     1256 },
            { "cp1257",     1257 },
            { "cp874",      874 },
            { "cp932",      932 },
            { "eucjp",      20932 },
            { "euckr",      51949 },
            { "gb18030",    54936 },
            { "gb2312",     936 },
            { "iso2022jp",  50220 },
            { "iso2022kr",  50225 },
            { "iso88591",   28591 },
            { "iso885913",  28603 },
            { "iso885915",  28605 },
            { "iso88592",   28592 },
            { "iso88593",   28593 },
            { "iso88594",   28594 },
            { "iso88595",   28595 },
            { "iso88596",   28596 },
            { "iso88597",   28597 },
            { "iso88598",   28598 },
            { "iso88599",   28599 },
            { "koi8r",      20866 },
            { "koi8u",      21866 },
            { "shiftjis",   932 },
            //{ "sjis",     932 },
            { "usascii",    20127 },
            { "utf8",       65001 },
            { "windows1250",1250 },
            { "windows1251",1251 },
            { "windows1252",1252 },
            { "windows1253",1253 },
            { "windows1254",1254 },
            { "windows1255",1255 },
            { "windows1256",1256 },
            { "windows1257",1257 },
            { "windows874", 874 },
            { "windows932", 932 },
        };

        char buffer[64];
        GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, buffer, sizeof(buffer));

        uint32 systemCodepage = boost::lexical_cast<uint32>(buffer);
        uint32 availableCodepageCount = sizeof(allWindowsEncodings) / sizeof(allWindowsEncodings[0]);

        for (uint32 index = 0; index < availableCodepageCount; ++index)
        {
            if (systemCodepage == allWindowsEncodings[index].codepage)
            {
                staticCurrentLocale = allWindowsEncodings[index].name;
                break;
            }
        }
#else
        staticCurrentLocale = "UTF-8";
#endif
    }

    return staticCurrentLocale.get();
}


RoString RoString::FromEucKr(const std::string& str)
{
#ifdef _WIN32
    return FromEncoding(str, "euckr");
#else
    // ICU's EUC-KR locale name
    return FromEncoding(str, "cp949");
#endif
}

std::string RoString::FromEucKrAsUtf8(const std::string& str)
{
    return FromEucKr(str).asUTF8();
}

RoString RoString::FromCurrentEncoding(const std::string& str)
{
    return boost::locale::conv::to_utf<char>(str, getCurrentLocale());
}

std::string RoString::ToCurrentEncoding(const RoString& str)
{
    return boost::locale::conv::from_utf(str.asWStr(), getCurrentLocale());
}

RoString RoString::FromEncoding(const std::string& str, const std::string& encodingName)
{
    return boost::locale::conv::to_utf<char>(str.data(), encodingName);
}

std::string RoString::ToEncoding(const RoString& str, const std::string& encodingName)
{
    return boost::locale::conv::from_utf(str.asWStr(), encodingName);
}


roINLINE RoString readUtf8FromStream(std::istream& stream, uint32 lengthInBytes)
{
    RoScopedArray<char> buffer(new char[lengthInBytes]);
    stream.read(buffer.get(), lengthInBytes);
    buffer[lengthInBytes-1] = 0;

    return boost::locale::conv::utf_to_utf<wchar_t>(buffer.get());
}

roINLINE RoString readUtf16LittleEndianFromStream(std::istream& stream, uint32 lengthInBytes)
{
    uint32 textSize = lengthInBytes / 2;
    RoScopedArray<uint16> buffer(new uint16[textSize]);
    stream.read((char*)buffer.get(), lengthInBytes);
    buffer[textSize-1] = 0;

    return boost::locale::conv::utf_to_utf<wchar_t>(buffer.get());
}

roINLINE RoString readUtf16BigEndianFromStream(std::istream& stream, uint32 lengthInBytes)
{
    uint32 textSize = lengthInBytes / 2;
    RoScopedArray<uint16> buffer(new uint16[textSize]);
    stream.read((char*)buffer.get(), lengthInBytes);
    buffer[textSize-1] = 0;

    for(uint32 pos = 0; pos < textSize; ++pos)
        buffer[pos] = (buffer[pos] >> 8) | (buffer[pos] << 8);

    return boost::locale::conv::utf_to_utf<wchar_t>(buffer.get());
}

roINLINE RoString readAnsiStringFromStream(std::istream& stream, uint32 lengthInBytes)
{
    RoScopedArray<char> buffer(new char[lengthInBytes]);
    stream.read(buffer.get(), lengthInBytes);
    buffer[lengthInBytes-1] = 0;

    return RoString::FromCurrentEncoding(buffer.get());
}

RoString RoString::FromStream(std::istream& stream)
{
    stream.seekg(0, std::ios::end);
    uint32 streamSize = (uint32)stream.tellg();
    stream.seekg(0, std::ios::beg);

    uint32 byteMarks = 0;
    stream.read((char*)&byteMarks, 4);

    if ((byteMarks & 0x00FFFFFF) == 0x00BFBBEF)
    {
        stream.seekg(3, std::ios::beg);
        return readUtf8FromStream(stream, streamSize - 3);
    }
    else if ((byteMarks & 0x0000FFFF) == 0x0000FEFF)
    {
        stream.seekg(2, std::ios::beg);
        return readUtf16LittleEndianFromStream(stream, streamSize - 2);
    }
    else if ((byteMarks & 0x0000FFFF) == 0x0000FFFE)
    {
        stream.seekg(2, std::ios::beg);
        return readUtf16BigEndianFromStream(stream, streamSize - 2);
    }

    stream.seekg(0, std::ios::beg);
    return readAnsiStringFromStream(stream, streamSize);
}

namespace boost
{
    size_t hash_value(const RoString& str)
    {
        return boost::hash_value(str.asUTF8());
    }
}

bool tbb::rostring_hasher::equal(const RoString& j, const RoString& k) const
{
    return j == k;
}

size_t tbb::rostring_hasher::hash(const RoString& key) const
{
    return hasher(key);
}
