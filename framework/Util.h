#pragma once

#define R3 1.73205

template <class T>
inline T distance2(T x1, T y1, T x2, T y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
