#include <math.h>
#include "UrsineMath.h"

namespace ursine
{
    INLINE Vec4::Vec4(void)
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(0.0f) { }

    INLINE Vec4::Vec4(const Vec4 &value)
        : x(value.X())
        , y(value.Y())
        , z(value.Z())
        , w(value.w) { }

    INLINE Vec4::Vec4(float value)
        : x(value)
        , y(value)
        , z(value)
        , w(value) { }

    INLINE Vec4::Vec4(float X, float Y, float Z, float W)
        : x(X)
        , y(Y)
        , z(Z)
        , w(W) { }

    // Properties
    INLINE const Vec4 &Vec4::Zero(void)
    {
        static const Vec4 zero;

        return zero;
    }

    INLINE const Vec4 &Vec4::One(void)
    {
        static const Vec4 one(1.0f);

        return one;
    }

    INLINE const Vec4 &Vec4::UnitX(void)
    {
        static const Vec4 unit_x(1.0f, 0.0f, 0.0f, 0.0f);

        return unit_x;
    }

    INLINE const Vec4 &Vec4::UnitY(void)
    {
        static const Vec4 unit_y(0.0f, 1.0f, 0.0f, 0.0f);

        return unit_y;
    }

    INLINE const Vec4 &Vec4::UnitZ(void)
    {
        static const Vec4 unit_z(0.0f, 0.0f, 1.0f, 0.0f);

        return unit_z;
    }

    INLINE const Vec4 &Vec4::UnitW(void)
    {
        static const Vec4 unit_w(0.0f, 0.0f, 0.0f, 1.0f);

        return unit_w;
    }
    
    // Public Methods
    INLINE void Vec4::Set(float X, float Y, float Z, float W)
    {
        x = X;
        y = Y;
        z = Z;
        w = W;
    }

    INLINE void Vec4::Clamp(const Vec4 &min, const Vec4 &max)
    {
        *this = Max(Min(*this, max), min);
    }

    INLINE Vec4 Vec4::Clamp(const Vec4 &value, const Vec4 &min, const Vec4 &max)
    {
        return Max(Min(value, max), min);
    }

    INLINE float Vec4::Distance(const Vec4 &other) const
    {
        return Distance(*this, other);
    }

    INLINE float Vec4::Distance(const Vec4 &vec1, const Vec4 &vec2)
    {
        return static_cast<float>(sqrt(DistanceSquared(vec1, vec2)));
    }

    INLINE float Vec4::DistanceSquared(const Vec4 &other) const
    {
        return DistanceSquared(*this, other);
    }

    INLINE float Vec4::DistanceSquared(const Vec4 &vec1, const Vec4 &vec2)
    {
        return (vec1 - vec2).LengthSquared();
    }

    INLINE float Vec4::Dot(const Vec4 &other) const
    {
        return Dot(*this, other);
    }

    INLINE float Vec4::Dot(const Vec4 &vec1, const Vec4 &vec2)
    {
        return vec1.X() * vec2.X() + vec1.Y() * vec2.Y() +
               vec1.Z() * vec2.Z() + vec1.w * vec2.w;
    }

    INLINE float Vec4::Length(void) const
    {
        return Length(*this);
    }

    INLINE float Vec4::Length(const Vec4 &vec)
    {
        return static_cast<float>(sqrt(LengthSquared(vec)));
    }

    INLINE float Vec4::LengthSquared(void) const
    {
        return LengthSquared(*this);
    }

    INLINE float Vec4::LengthSquared(const Vec4 &vec)
    {
        return Dot(vec, vec);
    }

    INLINE void Vec4::Max(const Vec4 &other)
    {
        x = math::Max(x, other.X());
        y = math::Max(y, other.Y());
        z = math::Max(z, other.Z());
        w = math::Max(w, other.w);
    }

    INLINE Vec4 Vec4::Max(const Vec4 &vec1, const Vec4 &vec2)
    {
        return{
            math::Max(vec1.X(), vec2.X()),
            math::Max(vec1.Y(), vec2.Y()),
            math::Max(vec1.Z(), vec2.Z()),
            math::Max(vec1.w, vec2.w)
        };
    }

    INLINE void Vec4::Min(const Vec4 &other)
    {
        x = math::Min(x, other.X());
        y = math::Min(y, other.Y());
        z = math::Min(z, other.Z());
        w = math::Min(w, other.w);
    }

    INLINE Vec4 Vec4::Min(const Vec4 &vec1, const Vec4 &vec2)
    {
        return{
            math::Min(vec1.X(), vec2.X()),
            math::Min(vec1.Y(), vec2.Y()),
            math::Min(vec1.Z(), vec2.Z()),
            math::Min(vec1.w, vec2.w)
        };
    }

    INLINE void Vec4::Normalize(void)
    {
        *this /= Length(*this);
    }

    INLINE Vec4 Vec4::Normalize(const Vec4 &vec)
    {
        return vec / Length(vec);
    }

    INLINE void Vec4::Reflect(const Vec4 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(*this, normal);

        *this -= normal * k;
    }

    INLINE Vec4 Vec4::Reflect(const Vec4 &vec, const Vec4 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(vec, normal);

        return vec - (normal * k);
    }

    // Accessors
    INLINE float Vec4::X(void) const
    {
        return x;
    }

    INLINE float Vec4::Y(void) const
    {
        return y;
    }

    INLINE float Vec4::Z(void) const
    {
        return z;
    }

    INLINE float Vec4::W(void) const
    {
        return w;
    }

    INLINE float &Vec4::X(void)
    {
        return x;
    }

    INLINE float &Vec4::Y(void)
    {
        return y;
    }

    INLINE float &Vec4::Z(void)
    {
        return z;
    }

    INLINE float &Vec4::W(void)
    {
        return w;
    }

    INLINE float Vec4::operator[](uint index) const
    {
        return (&x)[index];
    }

    INLINE float &Vec4::operator[](uint index)
    {
        return (&x)[index];
    }

    INLINE const float* Vec4::GetFloatPtr(void) const
    {
        return &x;
    }

    // Operators
    INLINE bool Vec4::operator==(const Vec4 &rhs) const
    {
        return math::IsEqual(x, rhs.X()) &&
               math::IsEqual(y, rhs.Y()) &&
               math::IsEqual(z, rhs.Z()) &&
               math::IsEqual(w, rhs.w);
    }

    INLINE bool Vec4::operator!=(const Vec4 &rhs) const
    {
        return !(*this == rhs);
    }

    INLINE Vec4 Vec4::operator+(const Vec4 &rhs) const
    {
        return {
            x + rhs.X(),
            y + rhs.Y(),
            z + rhs.Z(),
            w + rhs.w
        };
    }

    INLINE Vec4 Vec4::operator-(void) const
    {
        return {
            -x, -y, -z, -w
        };
    }

    INLINE Vec4 Vec4::operator-(const Vec4 &rhs) const
    {
        return {
            x - rhs.X(),
            y - rhs.Y(),
            z - rhs.Z(),
            w - rhs.w
        };
    }

    INLINE Vec4 Vec4::operator*(const Vec4 &rhs) const
    {
        return {
            x * rhs.X(),
            y * rhs.Y(),
            z * rhs.Z(),
            w * rhs.w
        };
    }

    INLINE Vec4 Vec4::operator*(float rhs) const
    {
        return {
            x * rhs,
            y * rhs,
            z * rhs,
            w * rhs
        };
    }

    INLINE Vec4 operator*(float lhs, const Vec4 &rhs)
    {
        return {
            rhs.X() * lhs,
            rhs.Y() * lhs,
            rhs.Z() * lhs,
            rhs.w * lhs
        };
    }

    INLINE Vec4 Vec4::operator/(const Vec4 &rhs) const
    {
        return {
            x / rhs.X(),
            y / rhs.Y(),
            z / rhs.Z(),
            w / rhs.w
        };
    }

    INLINE Vec4 Vec4::operator/(float rhs) const
    {
        float inv = 1.0f / rhs;

        return {
            x * inv,
            y * inv,
            z * inv,
            w * inv
        };
    }

    INLINE const Vec4 &Vec4::operator=(const Vec4 &rhs)
    {
        x = rhs.X();
        y = rhs.Y();
        z = rhs.Z();
        w = rhs.w;

        return *this;
    }

    INLINE const Vec4 &Vec4::operator+=(const Vec4 &rhs)
    {
        x += rhs.X();
        y += rhs.Y();
        z += rhs.Z();
        w += rhs.w;

        return *this;
    }

    INLINE const Vec4 &Vec4::operator-=(const Vec4 &rhs)
    {
        x -= rhs.X();
        y -= rhs.Y();
        z -= rhs.Z();
        w -= rhs.w;

        return *this;
    }

    INLINE const Vec4 &Vec4::operator*=(const Vec4 &rhs)
    {
        x *= rhs.X();
        y *= rhs.Y();
        z *= rhs.Z();
        w *= rhs.w;

        return *this;
    }

    INLINE const Vec4 &Vec4::operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;

        return *this;
    }

    INLINE const Vec4 &Vec4::operator/=(const Vec4 &rhs)
    {
        x /= rhs.X();
        y /= rhs.Y();
        z /= rhs.Z();
        w /= rhs.w;

        return *this;
    }

    INLINE const Vec4 &Vec4::operator/=(float rhs)
    {
        float inv = 1.0f / rhs;

        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;

        return *this;
    }
}
