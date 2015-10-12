#pragma once

#include <exception>
#include <functional>
#include <memory>

template <typename T>
class RoMaybe
{
public:
    struct NoOpDeleter
    {
        void operator()(RoMaybe<T>*) {}
    };
    using pointer_t = std::shared_ptr<RoMaybe<T>>;

    static pointer_t Nothing();
    static pointer_t Just(const T& value);

public:
    RoMaybe() = default;
    virtual ~RoMaybe() = default;

    virtual bool isNothing() const = 0;

    virtual T get() const = 0;

    T getOrElse(const T& defaultValue) const
    {
        if (isNothing())
        {
            return defaultValue;
        }
        return get();
    }

    T getOrThrow(const std::function<std::exception()>& exceptionSupplier) const
    {
        if (isNothing())
        {
            throw exceptionSupplier();
        }
        return get();
    }

    pointer_t bind(const std::function < T(const T&)>& func) const
    {
        return map<T>(func);
    }

    pointer_t flatBind(const std::function<pointer_t(const T&)>& func) const
    {
        return flatMap<T>(func);
    }

    template <typename U>
    typename RoMaybe<U>::pointer_t map(const std::function<U(const T&)>& func) const
    {
        return flatMap<U>([&](const T& val) { return RoMaybe<U>::Just(func(val)); });
    }

    template <typename U>
    typename RoMaybe<U>::pointer_t flatMap(const std::function<typename RoMaybe<U>::pointer_t(const T&)>& func) const
    {
        if (isNothing())
        {
            return RoMaybe<U>::Nothing();
        }
        return func(get());
    }
};

template <typename T>
class RoNothing : public RoMaybe<T>
{
public:
    virtual bool isNothing() const override
    {
        return true;
    }

    virtual T get() const override
    {
        throw std::runtime_error("No value has been set for this.");
    }
};

template <typename T>
class RoJust : public RoMaybe<T>
{
public:
    RoJust(const T& value)
        : mValue(value)
    {
    }

    virtual bool isNothing() const override
    {
        return false;
    }

    virtual T get() const override
    {
        return mValue;
    }

private:
    const T mValue;
};

template <typename T>
typename RoMaybe<T>::pointer_t RoMaybe<T>::Nothing()
{
    thread_local static RoNothing<T> nothingInstance;
    static typename RoMaybe<T>::pointer_t nothing(&nothingInstance, NoOpDeleter());
    return nothing;
}

template <typename T>
typename RoMaybe<T>::pointer_t RoMaybe<T>::Just(const T& value)
{
    return std::make_shared<RoJust<T>>(value);
}
