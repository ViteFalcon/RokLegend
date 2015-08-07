////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RoPrerequisites.h"

#include <boost/unordered_map.hpp>
#include <boost/utility/value_init.hpp>

namespace Detail
{
    using ConversionFn = std::function<bool(const void*, void*)>;

    class VariantConversionTable
    {
    public:
        VariantConversionTable(){}

        void add(const std::type_info& fromType, const std::type_info& toType, ConversionFn conversionFunction);

        bool canConvert(const std::type_info& fromType, const std::type_info& toType) const;

        template <typename ToType>
        bool convert(const std::type_info& fromType, const void* fromData, ToType& toData) const;

    private:
        template <typename key_type, typename value_type>
        using map = boost::unordered_map < key_type, value_type > ;

        using ToConversionMap = map<const std::type_info*, ConversionFn>;
        using ConversionMap = map<const std::type_info*, ToConversionMap>;

        ConversionMap mConversions;
    };
}

class RoVariant
{
private:
    class ValueHolder
    {
    public:
        ValueHolder(){}
        virtual ~ValueHolder(){}

        virtual const std::type_info& getType() const = 0;
        virtual ValueHolder* clone() const = 0;
        virtual const void* data() const = 0;
        virtual bool equals(const ValueHolder&) const = 0;
    };

public:
    template <typename T>
    class ValueHeld : public ValueHolder
    {
    public:
        using _MyType = ValueHeld<T>;

        ValueHeld(const T& val)
            :value(val)
        {
        }

        virtual const std::type_info& getType() const
        {
            return typeid(T);
        }
        virtual ValueHolder* clone() const
        {
            return new ValueHeld<T>(value);
        }
        virtual const void* data() const
        {
            return &value;
        }

        virtual bool equals(const ValueHolder& rhs) const
        {
            if (rhs.getType() == getType())
            {
                return value == static_cast<const _MyType &>(rhs).value;
            }
            return false;
        }

        T value;
    };

private:

    unique_ptr<ValueHolder> mHolder;

    static Detail::VariantConversionTable sConversionTable;
public:
    template <typename T>
    class Ref
    {
    public:
        explicit inline Ref(RoVariant& parent)
            : mParent(&parent)
        {
        }

        inline Ref(const Ref& rhs)
            : mParent(rhs.mParent)
        {
        }

        inline bool empty() const
        {
            return mParent->empty();
        }

        inline bool notNull() const
        {
            return (bool)(*mParent);
        }

        inline Ref& operator = (const T& val)
        {
            (*mParent) = val;
            return *this;
        }

        inline Ref& operator = (const Ref& rhs)
        {
            mParent = rhs.mParent;
            return *this;
        }

        inline operator T() const
        {
            return value();
        }

        inline RoVariant& parent()
        {
            return *mParent;
        }

        inline const RoVariant& parent() const
        {
            return *mParent;
        }

        inline T value() const
        {
            boost::initialized<T> val;
            mParent->as(val.data());
            return val;
        }

    private:
        RoVariant* mParent;
    };

    template <typename T>
    class ConstRef
    {
    public:
        inline ConstRef(const RoVariant& parent)
            : mParent(&parent)
        {
        }

        inline ConstRef(const ConstRef& rhs)
            : mParent(rhs.mParent)
        {
        }

        inline ConstRef(const Ref<T>& rhs)
            : mParent(&rhs.parent())
        {
        }

        inline bool empty() const
        {
            return mParent->empty();
        }

        inline bool notNull() const
        {
            return (bool)(*mParent);
        }

        inline ConstRef& operator = (const ConstRef& rhs)
        {
            mParent = rhs.mParent;
            return *this;
        }

        inline ConstRef& operator = (const Ref<T>& rhs)
        {
            mParent = &rhs.parent();
            return *this;
        }

        inline operator T() const
        {
            return value();
        }

        inline const RoVariant& parent() const
        {
            return *mParent;
        }

        inline T value() const
        {
            boost::initialized<T> val;
            mParent->as(val.data());
            return val;
        }

    private:
        const RoVariant* mParent;
    };

    /**
      Adds a function to the list of conversion functions that is capable of converting
      values from FromType to ToType. The conversion function should be of the form
      bool FunctionName(const void* inData, void* outData);
     **/
    template <typename FromType, typename ToType>
    static void RegisterConversionFunction(Detail::ConversionFn conversionFunction)
    {
        sConversionTable.add(typeid(FromType), typeid(ToType), conversionFunction);
    }


    /**
      This function verifies if RoVariant class has the capability of converting FromType
      values to ToType values.
     **/
    template <typename FromType, typename ToType>
    static bool CanConvert()
    {
        return sConversionTable.canConvert(typeid(FromType), typeid(ToType));
    }

    /**
      Default constructor
     **/
    inline RoVariant()
        :mHolder(nullptr)
    {
    }

    /**
      Constructor that converts char-pointer to std::string
     **/
    inline explicit RoVariant(const char* val)
        :mHolder(new ValueHeld<RoString>(val))
    {
    }

    /**
      Constructor that converts wide-char-pointer to std::wstring
     **/
    inline explicit RoVariant(const wchar_t* val)
        :mHolder(new ValueHeld<RoString>(val))
    {
    }

    /**
    Constructor that converts std::string to RoString
    **/
    inline explicit RoVariant(const std::string& val)
        :mHolder(new ValueHeld < RoString > { val })
    {
    }

    /**
    Constructor that converts std::wstring to RoString
    **/
    inline explicit RoVariant(const std::wstring& val)
        :mHolder(new ValueHeld < RoString > { val })
    {
    }

    /**
      Constructor that accepts any type of value to be stored
     **/
    template <typename T>
    inline explicit RoVariant(const T& val)
        :mHolder(new ValueHeld<T>(val))
    {
    }

    /**
      Copy constructor
     **/
    inline RoVariant(const RoVariant& other)
        :mHolder(other.mHolder->clone())
    {
    }

    /**
      Destructor
     **/
    inline ~RoVariant()
    {
    }

    /**
      Swap values of this instance with another RoVariant
     **/
    inline void swap(RoVariant& other)
    {
        std::swap(mHolder, other.mHolder);
    }

    /**
      Assignment operator
     **/
    inline RoVariant& operator = (const RoVariant& rhs)
    {
        swap(RoVariant(rhs));
        return *this;
    }

    /**
      Assignment operator that accepts any type of value to be stored.
     **/
    template <typename T>
    inline RoVariant& operator = (const T& rhs)
    {
        swap(RoVariant(rhs));
        return *this;
    }

    /**
      Returns the type of the value stored. If no values are stored
      it will return the typeid of void.
     **/
    inline const std::type_info& getType() const
    {
        if (mHolder)
            return mHolder->getType();
        return typeid(void);
    }

    template <typename T>
    inline bool isType() const
    {
        return typeid(T) == getType();
    }

    /**
      Return if this variant has a value stored.
     **/
    inline bool empty() const
    {
        return nullptr == mHolder;
    }

    /**
      Returns true if variant has a value assigned to it. False otherwise.
     **/
    inline operator bool () const
    {
        return !empty();
    }

    /**
      Check if another variant type is equal to this instance.
     **/
    inline bool equals(const RoVariant& rhs) const
    {
        if (mHolder)
            return mHolder->equals(*rhs.mHolder);
        return false;
    }

    /**
      Equality operator
     **/
    inline bool operator == (const RoVariant& rhs) const
    {
        return equals(rhs);
    }

    /**
      Inequality operator
     **/
    inline bool operator != (const RoVariant& rhs) const
    {
        return !equals(rhs);
    }

    /**
      Less than operator.
     **/
    inline bool operator < (const RoVariant& rhs) const
    {
        return mHolder < rhs.mHolder;
    }

    template <typename T>
    inline Ref<T> as()
    {
        return Ref<T>(*this);
    }

    template <typename T>
    inline ConstRef<T> as() const
    {
        return ConstRef<T>(*this);
    }

    template <typename T>
    inline void as(T& val) const
    {
        if (mHolder)
        {
            if (getType() == typeid(T))
            {
                val = static_cast<const ValueHeld<T>*>(mHolder.get())->value;
            } else if (!sConversionTable.convert<T>(mHolder->getType(), mHolder->data(), val)) {
                std::stringstream errorMsg;
                errorMsg<<"Failed to convert RoVariant from '"<<getType().name()<<"' to '"<<typeid(T).name()<<"'!";
                throw std::runtime_error(errorMsg.str().c_str());
            }
        }
    }

    inline bool toBool() const
    {
        return as<bool>().value();
    }
    inline char toChar() const
    {
        return as<char>().value();
    }
    inline unsigned char toUChar() const
    {
        return as<unsigned char>().value();
    }
    inline int toInt() const
    {
        return as<int>().value();
    }
    inline unsigned int toUInt() const
    {
        return as<unsigned int>().value();
    }
    inline float toFloat() const
    {
        return as<float>().value();
    }
    inline std::string toString() const
    {
        return as<std::string>().value();
    }
    inline std::wstring toWideString() const
    {
        return as<std::wstring>().value();
    }

    friend std::ostream& operator << (std::ostream& stream, const RoVariant& var)
    {
        if (var.empty())
        {
            stream<<"RoVariant[void]";
        } else {
            std::string str = var.toString();
            if (str.empty())
            {
                stream<<"RoVariant["<<var.getType().name()<<"]";
            } else {
                stream<<str;
            }
        }
        return stream;
    }
};

namespace Detail {
    template <typename ToType>
    bool VariantConversionTable::convert(const std::type_info& fromType, const void* fromData, ToType& toData) const
    {
        if (fromData)
        {
            ConversionMap::const_iterator from_itr = mConversions.find(&fromType);
            if (mConversions.end() != from_itr)
            {
                ToConversionMap::const_iterator to_itr = from_itr->second.find(&typeid(ToType));
                if (from_itr->second.end() != to_itr)
                {
                    const ConversionFn& converter = to_itr->second;
                    return converter(fromData, (void*)&toData);
                }
            }
        }
        return false;
    }
}
