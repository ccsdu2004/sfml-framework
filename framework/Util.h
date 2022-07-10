#pragma once
#include <cmath>
#include <string>
#include <utility>
#include <array>
#include <boost/mp11.hpp>
#include <boost/describe/enum.hpp>
#include <boost/describe/enumerators.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

template <class T>
inline T distance2(T x1, T y1, T x2, T y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

inline float distance(const sf::Vector2f &a, const sf::Vector2f &b)
{
    return std::sqrt(distance2(a.x, a.y, b.x, b.y));
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
inline bool isEqual(T a, T b, T t = 1e-6)
{
    return std::fabs(a - b) < t;
}

inline sf::Vector2f getRectCenter(const sf::FloatRect& area)
{
    return sf::Vector2f(area.left + area.width * 0.5f, area.top + area.height * 0.5f);
}

template<class T>
struct Range {
    Range()
    {}

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

template<class E> struct enum_descriptor {
    E value;
    char const * name;
};

template<class E, template<class... T> class L, class... T>
constexpr std::array<enum_descriptor<E>, sizeof...(T)>
describe_enumerators_as_array_impl( L<T...> )
{
    return { { { T::value, T::name }... } };
}

template<class E> constexpr auto describe_enumerators_as_array()
{
    return describe_enumerators_as_array_impl<E>( boost::describe::describe_enumerators<E>() );
}

inline float clipAngle(float angle)
{
    if (angle > 360.0)
        return angle - 360;
    else if (angle < 0)
        return 360 + angle;
    return angle;
}

inline bool shouldRandDoIt(int p)
{
    int value = rand() % 100 + 1;
    return p >= value;
}

inline sf::Color blendColors(const sf::Color &firstColor, const sf::Color &secondColor,
                             float interpolation = 0.5f)
{
    interpolation = clamp(0.0f, 1.0f, interpolation);
    float p = 1.f - interpolation;
    return sf::Color(
               static_cast<sf::Uint8>(p * firstColor.r + interpolation * secondColor.r),
               static_cast<sf::Uint8>(p * firstColor.g + interpolation * secondColor.g),
               static_cast<sf::Uint8>(p * firstColor.b + interpolation * secondColor.b),
               static_cast<sf::Uint8>(p * firstColor.a + interpolation * secondColor.a));

}

sf::FloatRect expandRect(const sf::FloatRect& a, const sf::FloatRect& b);

