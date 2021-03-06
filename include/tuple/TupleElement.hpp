#pragma once

#include <type_traits>
#include <utility>

namespace IDragnev::Detail
{
    template <typename T>
    inline constexpr auto canBeInherited = std::is_class_v<T> && !std::is_final_v<T>;

    template <std::size_t Height,
              typename T,
              bool = canBeInherited<T>
    > class TupleElement;

    template <std::size_t Height, typename T>
    class TupleElement<Height, T, false>
    {
    public:
        TupleElement() = default;
        template<typename U>
        constexpr TupleElement(U&& value) : value(std::forward<U>(value)) {}

        constexpr T&& get() && noexcept { return std::move(value); }
        constexpr const T&& get() const && noexcept { return std::move(value); }
        constexpr T& get() & noexcept { return value; }
        constexpr const T& get() const & noexcept { return value; }

    private:
        T value;
    };

    template <std::size_t Height, typename T>
    class TupleElement<Height, T, true> : private T
    {
    public:
        TupleElement() = default;
        template<typename U>
        constexpr TupleElement(U&& value) : T(std::forward<U>(value)) {}

        constexpr T&& get() && noexcept { return std::move(*this); }
        constexpr const T&& get() const && noexcept { return std::move(*this); }
        constexpr T& get() & noexcept { return *this; }
        constexpr const T& get() const & noexcept { return *this; }
    };

    template <std::size_t Height, typename T>
    inline constexpr
    const T& getValue(const TupleElement<Height, T>& e) noexcept
    {
        return e.get();
    }

    template <std::size_t Height, typename T>
    inline constexpr
    T& getValue(TupleElement<Height, T>& e) noexcept
    {
        return const_cast<T&>(getValue(std::as_const(e)));
    }

    template <std::size_t Height, typename T>
    inline constexpr
    const T&& getValue(const TupleElement<Height, T>&& e) noexcept
    {
        return std::move(getValue(e));
    }

    template <std::size_t Height, typename T>
    inline constexpr
    T&& getValue(TupleElement<Height, T>&& e) noexcept
    {
        return std::move(e).get();
    }
} //namespace IDragnev::Detail