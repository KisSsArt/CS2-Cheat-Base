#pragma once

#include <cmath>

#define ZERO Vector{ 0.f, 0.f, 0.f }

struct Vector
{
    constexpr auto notNull() const noexcept
    {
        return x || y || z;
    }

    constexpr auto operator==(const Vector& v) const noexcept
    {
        return x == v.x && y == v.y && z == v.z;
    }

    constexpr auto operator!=(const Vector& v) const noexcept
    {
        return !(*this == v);
    }

    constexpr Vector& operator+=(const Vector& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    constexpr Vector& operator+=(float f) noexcept
    {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    constexpr Vector& operator-=(const Vector& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    constexpr Vector& operator-=(float f) noexcept
    {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    constexpr auto operator-(const Vector& v) const noexcept
    {
        return Vector{ x - v.x, y - v.y, z - v.z };
    }

    constexpr auto operator+(const Vector& v) const noexcept
    {
        return Vector{ x + v.x, y + v.y, z + v.z };
    }

    constexpr auto operator*(const Vector& v) const noexcept
    {
        return Vector{ x * v.x, y * v.y, z * v.z };
    }

    constexpr Vector& operator*=(float mul) noexcept
    {
        x *= mul;
        y *= mul;
        z *= mul;
        return *this;
    }

    constexpr Vector& operator/=(float div) noexcept
    {
        float oofl = 1.0f / div;
        x *= oofl;
        y *= oofl;
        z *= oofl;
        return *this;
    }

    constexpr Vector operator/(float fl) const
    {
        Vector res;
        res.x = x / fl;
        res.y = y / fl;
        res.z = z / fl;
        return res;
    }

    constexpr auto operator*(float mul) const noexcept
    {
        return Vector{ x * mul, y * mul, z * mul };
    }

    constexpr auto operator-(float sub) const noexcept
    {
        return Vector{ x - sub, y - sub, z - sub };
    }

    constexpr auto operator+(float add) const noexcept
    {
        return Vector{ x + add, y + add, z + add };
    }

    float& operator[](int i) noexcept
    {
        return ((float*)this)[i];
    }

    float operator[](int i) const
    {
        return ((float*)this)[i];
    }

    bool operator < (const Vector& v) {
        return { this->x < v.x&&
                 this->y < v.y&&
                 this->z < v.z };
    }

    bool operator > (const Vector& v) {
        return { this->x > v.x &&
                 this->y > v.y &&
                 this->z > v.z };
    }

    bool operator<=(const Vector& v) {
        return { this->x <= v.x &&
                 this->y <= v.y &&
                 this->z <= v.z };
    }

    bool operator>=(const Vector& v) {
        return { this->x >= v.x &&
                 this->y >= v.y &&
                 this->z >= v.z };
    }

    bool roughlyEqual(const Vector v, float difference = 1.f) const noexcept
    {
        if ((v.x - difference <= x) && (x <= v.x + difference) && (v.y - difference <= y) && (y <= v.y + difference) && (v.z - difference <= z) && (z <= v.z + difference))
            return true;
        return false;
    }

    bool is_valid() const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }

    void Zero() noexcept
    {
        x = y = z = 0.0f;
    }

    bool isZero() const noexcept
    {
        return x == 0.0f && y == 0.0f && z == 0.0f;
    }

    Vector normalized() noexcept
    {
        auto vectorNormalize = [](Vector& vector)
        {
            float radius = std::sqrtf(std::powf(vector.x, 2) + std::powf(vector.y, 2) + std::powf(vector.z, 2));
            radius = 1.f / (radius + FLT_EPSILON);

            vector *= radius;

            return radius;
        };
        Vector v = *this;
        vectorNormalize(v);
        return v;
    }

    Vector normalize() noexcept
    {
        x = std::isfinite(x) ? std::remainder(x, 360.0f) : 0.0f;
        y = std::isfinite(y) ? std::remainder(y, 360.0f) : 0.0f;
        z = 0.0f;
        return *this;
    }

    auto distance(const Vector& v) const noexcept
    {
        return std::hypot(x - v.x, y - v.y, z - v.z);
    }

    auto length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    auto length2D() const
    {
        return std::sqrt(x * x + y * y);
    }

    auto squareLength() const
    {
        return x * x + y * y + z * z;
    }

    float squareLength2D() const
    {
        return (x * x + y * y);
    }

    constexpr auto dotProduct(const Vector& v) const noexcept
    {
        return (x * v.x + y * v.y + z * v.z);
    }

    constexpr auto dotProduct(const float* f) const noexcept
    {
        return (x * f[0] + y * f[1] + z * f[2]);
    }

    Vector Normalized() const
    {
        Vector res = *this;
        float l = res.length();
        if (l) res /= l;
        else res.x = res.y = res.z = 0.0f;
        return res;
    }

    Vector Cross(const Vector& vOther) const noexcept;

    Vector normalizeInPlace() noexcept
    {
        float iradius = 1.0f / (length() + FLT_EPSILON);
        x *= iradius;
        y *= iradius;
        z *= iradius;
        return Vector(x, y, z);
    }

    auto distTo(const Vector& v) const noexcept
    {
        return (*this - v).length();
    }

    float Normalize() const
    {
        Vector res = *this;
        float l = res.length();
        if (l)
            res /= l;
        else
            res.x = res.y = res.z = 0.0f;

        return l;
    }

    void Init(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float x, y, z;
};

inline Vector CrossProduct(const Vector& a, const Vector& b)noexcept
{
    return Vector{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

inline Vector Vector::Cross(const Vector& vOther) const noexcept
{
    return CrossProduct(*this, vOther);
}