/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#ifndef NEXILIS_VECTOR2_HH
#define NEXILIS_VECTOR2_HH

#include <nexilis/types/vector.hh>

#include <cmath>
#include <cstdint>

namespace nexilis
{

template <typename T>
class Vector2 : public Vector
{
public:
    T x;
    T y;

    Vector2()
        : x(0), y(0)
    {
    }
    Vector2(T x, T y)
        : x(x), y(y)
    {
    }

    template <typename U>
    Vector2(const Vector2<U>& vec)
        : x(static_cast<T>(vec.x)),
          y(static_cast<T>(vec.y))
    {
    }

    /// Vector::getType implementation.
    VectorType getType() override
    {
        return VectorType::vector2;
    }

    bool operator!=(const Vector2<T>& rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }
    bool operator==(const Vector2<T>& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool operator<=(const Vector2<T>& rhs) const
    {
        return !(rhs < *this);
    }
    bool operator>=(const Vector2<T>& rhs) const
    {
        return !(*this < rhs);
    }
    bool operator<(const Vector2<T>& rhs) const
    {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }
    bool operator>(const Vector2<T>& rhs) const
    {
        return rhs < *this;
    }

    template <typename V>
    Vector2<T> operator/(const Vector2<V>& rhs) const
    {
        return Vector2<T>(x / rhs.x, y / rhs.y);
    }
    template <typename V>
    Vector2<T> operator*(const Vector2<V>& rhs) const
    {
        return Vector2<T>(x * rhs.x, y * rhs.y);
    }
    template <typename V>
    Vector2<T> operator+(const Vector2<V>& rhs) const
    {
        return Vector2<T>(x + rhs.x, y + rhs.y);
    }
    template <typename V>
    Vector2<T> operator-(const Vector2<V>& rhs) const
    {
        return Vector2<T>(x - rhs.x, y - rhs.y);
    }

    Vector2<T> operator/(const T rhs) const
    {
        return Vector2<T>(x / rhs, y / rhs);
    }
    Vector2<T> operator*(const T rhs) const
    {
        return Vector2<T>(x * rhs, y * rhs);
    }

    template <typename V>
    Vector2<T> operator*=(const Vector2<V>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }
    template <typename V>
    Vector2<T> operator/=(const Vector2<V>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }
    template <typename V>
    Vector2<T> operator+=(const Vector2<V>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    template <typename V>
    Vector2<T> operator-=(const Vector2<V>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    template <typename N>
    Vector2<N> as() const
    {
        return Vector2<N>(static_cast<N>(x), static_cast<N>(y));
    }
    Vector2<T> abs() const
    {
        return Vector2<T>(std::abs(x), std::abs(y));
    }
};

using Vector2f = Vector2<float>;
using Vector2u = Vector2<uint64_t>;
using Vector2i = Vector2<int>;

} // namespace nexilis

#endif
