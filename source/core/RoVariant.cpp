////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoVariant.h"
#include "RoStringUtil.h"
#include "RoLog.h"

#include <sstream>
#include <typeinfo>

#ifdef _MSC_VER
#    pragma warning(disable: 4800)
#endif // _MSC_VER

/** Helper Macros **/
#define POD_CONVERSION_FUNC_NAME(FromType, ToType) Detail::VariantConvert<FromType, ToType>
#define POD_CONVERT_TO_STR_FUNC_NAME(FromType) Detail::VariantConvertToStr<FromType>
#define CONVERT_TO_ROSTRING_FUNC_NAME(FromType) Detail::VariantConvertToRoString<FromType>
#define POD_CONVERT_TO_WSTR_FUNC_NAME(FromType) Detail::VariantConvertToWStr<FromType>

/** Registration Helper Macros **/
#define POD_CONVERSION_FUNC_REGISTER(FromType, ToType) (typeid(FromType), typeid(ToType), POD_CONVERSION_FUNC_NAME(FromType, ToType))
#define POD_CONVERSION_FUNC_REGISTER2(FromType, ToType) POD_CONVERSION_FUNC_REGISTER(FromType, ToType)\
    POD_CONVERSION_FUNC_REGISTER(FromType, unsigned ToType)

#define POD_CONVERT_TO_STR_FUNC_REGISTER(FromType) (typeid(FromType), typeid(std::string), POD_CONVERT_TO_STR_FUNC_NAME(FromType))
#define CONVERT_TO_ROSTRING_FUNC_REGISTER(FromType) (typeid(FromType), typeid(RoString), CONVERT_TO_ROSTRING_FUNC_NAME(FromType))
#define POD_CONVERT_TO_WSTR_FUNC_REGISTER(FromType) (typeid(FromType), typeid(std::wstring), POD_CONVERT_TO_WSTR_FUNC_NAME(FromType))

#define POD_CONVERSION_FUNC_REGISTER_ALL_POD(FromType) POD_CONVERSION_FUNC_REGISTER(FromType, bool)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, char)\
    POD_CONVERSION_FUNC_REGISTER(FromType, int8)\
    POD_CONVERSION_FUNC_REGISTER(FromType, uint8)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, short)\
    POD_CONVERSION_FUNC_REGISTER(FromType, int16)\
    POD_CONVERSION_FUNC_REGISTER(FromType, uint16)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, int)\
    POD_CONVERSION_FUNC_REGISTER(FromType, int32)\
    POD_CONVERSION_FUNC_REGISTER(FromType, uint32)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, long)\
    POD_CONVERSION_FUNC_REGISTER2(FromType, long long)\
    POD_CONVERSION_FUNC_REGISTER(FromType, int64)\
    POD_CONVERSION_FUNC_REGISTER(FromType, uint64)\
    POD_CONVERSION_FUNC_REGISTER(FromType, float)\
    POD_CONVERSION_FUNC_REGISTER(FromType, double)\
    POD_CONVERT_TO_STR_FUNC_REGISTER(FromType)\
    CONVERT_TO_ROSTRING_FUNC_REGISTER(FromType)\
    POD_CONVERT_TO_WSTR_FUNC_REGISTER(FromType)

#define POD_CONVERSION_FUNC_REGISTER_ALL_POD2(FromType) POD_CONVERSION_FUNC_REGISTER_ALL_POD(FromType)\
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(unsigned FromType)

namespace Detail
{
    namespace{
        class ConversionTableCreate
        {
        private:
            VariantConversionTable table;
        public:
            ConversionTableCreate(const std::type_info& fromType, const std::type_info& toType, ConversionFn conversionFunc)
            {
                table.add(fromType, toType, conversionFunc);
            }
            ConversionTableCreate& operator()(const std::type_info& fromType, const std::type_info& toType, ConversionFn conversionFunc)
            {
                table.add(fromType, toType, conversionFunc);
                return *this;
            }

            operator VariantConversionTable()
            {
                return table;
            }
        };

        template <typename T>
        T& CastToRef(void* ptr)
        {
            return *static_cast<T*>(ptr);
        }

        template <typename T>
        const T& CastToRef(const void* ptr)
        {
            return *static_cast<const T*>(ptr);
        }
    }

    void VariantConversionTable::add(const std::type_info& fromType, const std::type_info& toType, ConversionFn conversionFunction)
    {
        if (conversionFunction)
        {
            mConversions[&fromType][&toType] = conversionFunction;
        }
    }

    bool VariantConversionTable::canConvert(const std::type_info& fromType, const std::type_info& toType) const
    {
        ConversionMap::const_iterator from_itr = mConversions.find(&fromType);
        if (mConversions.end() != from_itr)
        {
            return from_itr->second.count(&toType) != 0;
        }
        return false;
    }

    template <typename FromType, typename ToType>
    bool VariantConvert(const void* inData, void* outData)
    {
        CastToRef<ToType>(outData) = static_cast<ToType>(CastToRef<FromType>(inData));
        return true;
    }

    template <typename FromType>
    bool VariantConvertToRoString(const void* inData, void *outData)
    {
        std::stringstream ss;
        ss << CastToRef<FromType>(inData);
        CastToRef<RoString>(outData) = RoString{ ss.str() };
        return true;
    }

    template <>
    bool VariantConvertToRoString<uint8>(const void* inData, void *outData)
    {
        const uint8 data = CastToRef<uint8>(inData);
        std::stringstream ss;
        ss << (int)data;
        CastToRef<RoString>(outData) = RoString{ ss.str() };
        return true;
    }

    template <>
    bool VariantConvertToRoString<std::string>(const void* inData, void* outData)
    {
        CastToRef<RoString>(outData) = CastToRef<std::string>(inData);
        return true;
    }

    template <>
    bool VariantConvertToRoString<std::wstring>(const void* inData, void* outData)
    {
        auto wideString = CastToRef<std::wstring>(inData);
        CastToRef<RoString>(outData) = RoString{ wideString };
        return true;
    }

    template <typename ToType>
    bool VariantConvertFromRoString(const void* inData, void *outData)
    {
        const RoString& str = CastToRef<RoString>(inData);
        std::istringstream ss(str.asUTF8());
        ToType& outValue = CastToRef<ToType>(outData);
        ss >> outValue;
        return !ss.fail();
    }

    template <>
    bool VariantConvertFromRoString<bool>(const void* inData, void* outData)
    {
        const RoString& str = CastToRef<RoString>(inData);
        RoOptionalBool val = RoStringUtil::ParseBool(str);
        if (!val.is_initialized())
        {
            return false;
        }
        CastToRef<bool>(outData) = val.get();
        return true;
    }

    template <>
    bool VariantConvertFromRoString<std::string>(const void* inData, void* outData)
    {
        CastToRef<std::string>(outData) = CastToRef<RoString>(inData);
        return true;
    }

    template <>
    bool VariantConvertFromRoString<std::wstring>(const void* inData, void* outData)
    {
        CastToRef<std::wstring>(outData) = CastToRef<RoString>(inData);
        return true;
    }

    template <typename FromType>
    bool VariantConvertToStr(const void *inData, void *outData)
    {
        std::ostringstream ss;
        ss << CastToRef<FromType>(inData);
        RoString string{ ss.str() };
        CastToRef<std::string>(outData) = string.asUTF8();
        return true;
    }

    template <typename FromType>
    bool VariantConvertToWStr(const void *inData, void *outData)
    {
        std::wostringstream ss;
        ss << CastToRef<FromType>(inData);
        CastToRef<std::wstring>(outData) = ss.str().c_str();
        return true;
    }

    template <typename ToType>
    bool VariantConvertFromStr(const void* inData, void* outData)
    {
        std::istringstream ss(CastToRef<std::string>(inData));
        ToType& outValue = CastToRef<ToType>(outData);
        ss >> outValue;
        return !ss.fail();
    }

    template <>
    bool VariantConvertFromStr<bool>(const void *inData, void *outData)
    {
        static const RoString True{ L"true" };
        static const RoString Yes{ L"yes" };
        static const RoString False{ L"false" };
        static const RoString No{ "no" };
        auto inValue = CastToRef<RoString>(inData);
        if (inValue.empty())
        {
            return false;
        }
        auto val = RoStringUtil::ToLower(inValue);
        auto outValue = CastToRef<bool>(outData);
        if (True == val || Yes == val || val == "1")
        {
            outValue = true;
            return true;
        } else if (False == val || No == val || val == "0") {
            outValue = false;
            return true;
        }
        return false;
    }

    bool VariantConvertWStrToStr(const void* inData, void* outData)
    {
        auto inWString = CastToRef<RoString>(inData);
        auto outString = CastToRef<std::string>(outData);
        outString = inWString.asUTF8();
        return true;
    }

    bool VariantConvertStrToWStr(const void* inData, void* outData)
    {
        auto inWString = CastToRef<RoString>(inData);
        auto outString = CastToRef<std::wstring>(outData);
        outString = inWString.asWStr();
        return true;
    }

#define CONVERT_FROM_ROSTRING_FUNC_REGISTER(ToType) (typeid(RoString), typeid(ToType), Detail::VariantConvertFromRoString<ToType>)
#define POD_CONVERT_FROM_STR_FUNC_REGISTER(ToType) (typeid(std::string), typeid(ToType), Detail::VariantConvertFromStr<ToType>)\
    CONVERT_FROM_ROSTRING_FUNC_REGISTER(ToType)
#define POD_CONVERT_FROM_STR_FUNC_REGISTER2(ToType) POD_CONVERT_FROM_STR_FUNC_REGISTER(ToType)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER(unsigned ToType)
#define POD_CONVERT_FROM_STR_FUNC_REGISTER_ALL POD_CONVERT_FROM_STR_FUNC_REGISTER(bool)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(char)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(short)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(int)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(long)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER2(long long)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER(float)\
    POD_CONVERT_FROM_STR_FUNC_REGISTER(double)
}

Detail::VariantConversionTable RoVariant::sConversionTable = Detail::ConversionTableCreate
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(bool)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(char)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(int8)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(uint8)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(short)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(int16)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(uint16)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(int)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(int32)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(uint32)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(long)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD2(long long)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(int64)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(uint64)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(float)
    POD_CONVERSION_FUNC_REGISTER_ALL_POD(double)
    POD_CONVERT_FROM_STR_FUNC_REGISTER_ALL
    CONVERT_FROM_ROSTRING_FUNC_REGISTER(std::string)
    CONVERT_FROM_ROSTRING_FUNC_REGISTER(std::wstring)
    CONVERT_TO_ROSTRING_FUNC_REGISTER(std::string)
    CONVERT_TO_ROSTRING_FUNC_REGISTER(std::wstring)
    (typeid(std::string), typeid(std::wstring), Detail::VariantConvertStrToWStr)
    (typeid(std::wstring), typeid(std::string), Detail::VariantConvertWStrToStr);

void RoVariant::throwConversionError(const std::type_info& from, const std::type_info& to) const
{
    std::stringstream errorMsg;
    errorMsg << "Failed to convert RoVariant from '" << from.name() << "' to '" << to.name() << "'!";
    roLOG_ERR << errorMsg.str();
    throw std::runtime_error(errorMsg.str().c_str());
}

#include <boost/algorithm/string.hpp>

bool RoVariant::isSequentialContainer() const
{
    static const std::string VECTOR_TYPE = "class std::vector";
    const std::string typeName(getType().name());
    return boost::algorithm::starts_with(typeName, VECTOR_TYPE);
}

