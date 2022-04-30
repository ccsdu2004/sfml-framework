#pragma once
#include <cmath>
#include <utility>
#include <boost/mp11.hpp>
#include <boost/describe/enum.hpp>
#include <boost/describe/enumerators.hpp>
#include <SFML/System/Vector2.hpp>

#define R3 1.73205

template <class T>
inline T distance2(T x1, T y1, T x2, T y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

template<class T>
inline T clamp(T value, T low, T high)
{
    if (value < low)
        return low;
    else if (value > high)
        return high;
    return value;
}

inline auto dot(const sf::Vector2f &lhs, const sf::Vector2f &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

inline auto length2(const sf::Vector2f &vector) noexcept
{
    return dot(vector, vector);
}

inline auto length(const sf::Vector2f &vector) noexcept
{
    return std::sqrt(length2(vector));
}

template<class T>
inline bool isEqual(T a, T b)
{
    return std::fabs(a - b) < 1e-6;
}

template<class T>
struct Range {
    Range() {}

    Range(T inputLow, T inputHigh):
        low(inputLow),
        high(inputHigh)
    {}

    Range(const Range &other):
        low(other.low),
        high(other.high)
    {}

    Range(const Range &&other):
        low(std::move(other.low)),
        high(std::move(other.high))
    {
    }

    Range operator= (const Range<T> &other)
    {
        low = other.low;
        high = other.high;
        return *this;
    }

    bool isValid()const
    {
        return low <= high;
    }

    bool isFixed()const
    {
        return isEqual<T>(low, high);
    }

    T low, high;
};

using RangeFloat = Range<float>;

template<class T>
inline T clamp(T value, const Range<T> &range)
{
    return clamp(value, range.low, range.high);
}

inline constexpr auto PI = 3.1415926f;
inline constexpr auto TO_RADIAN = PI / 180.0f;
inline constexpr auto TO_DEGREE = 180.0f / PI;

template<class E>
std::string toString(E e)
{
    std::string string;
    boost::mp11::mp_for_each< boost::describe::describe_enumerators<E> >([&](auto U) {
        if (U.value == e)
            string = U.name;
    });
    return string;
}

template<class E>
E fromString(const std::string &string, E defaultE)
{
    E value = defaultE;
    boost::mp11::mp_for_each< boost::describe::describe_enumerators<E> >([&](auto U) {
        if (string == U.name)
            value = U.value;
    });
    return value;
}
