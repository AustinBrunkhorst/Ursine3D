#include <math.h>
#include "UrsineMath.h"

namespace Ursine
{
    INLINE Vec3::Vec3(float X, float Y, float Z)
            : x(X)
            , y(Y)
            , z(Z)
    { }

    INLINE Vec3::Vec3(float value)
            : x(value)
            , y(value)
            , z(value)
    { }

    INLINE Vec3::Vec3(const Vec3 &value)
            : x(value.X())
            , y(value.Y())
            , z(value.Z())
    { }

    INLINE Vec3::Vec3(void)
            : x(0.0f)
            , y(0.0f)
            , z(0.0f)
    { }

    // Properties
    INLINE const Vec3 &Vec3::Zero(void)
    {
        static const Vec3 zero;

        return zero;
    }

    INLINE const Vec3 &Vec3::One(void)
    {
        static const Vec3 one(1.0f);

        return one;
    }

    INLINE const Vec3 &Vec3::UnitX(void)
    {
        static const Vec3 unit_x(1.0f, 0.0f, 0.0f);

        return unit_x;
    }

    INLINE const Vec3 &Vec3::UnitY(void)
    {
        static const Vec3 unit_y(0.0f, 1.0f, 0.0f);

        return unit_y;
    }

    INLINE const Vec3 &Vec3::UnitZ(void)
    {
        static const Vec3 unit_z(0.0f, 0.0f, 1.0f);

        return unit_z;
    }

    // Public Methods
    INLINE void Vec3::Set(float X, float Y, float Z)
    {
        x = X;
        y = Y;
        z = Z;
    }

    INLINE void Vec3::Clamp(const Vec3 &min, const Vec3 &max)
    {
        *this = Max(Min(*this, max), min);
    }

    INLINE Vec3 Vec3::Clamp(const Vec3 &value, const Vec3 &min, const Vec3 &max)
    {
        return Max(Min(value, max), min);
    }

    INLINE void Vec3::Cross(const Vec3 &other)
    {
        float original_x = x;
        float original_y = y;

        x = y          * other.Z() - other.Y() * z;
        y = z          * other.X() - other.Z() * original_x;
        z = original_x * other.Y() - other.X() * original_y;
    }

    INLINE float Vec3::TripleProduct(const Vec3& a, const Vec3& b, const Vec3& c)
    {
        return( (a.X() * (b.Y() * c.Z() - b.Z() * c.Y())) +
                (a.Y() * (b.Z() * c.X() - b.X() * c.Z())) +
                (a.Z() * (b.X() * c.Y() - b.Y() * c.X())) );
    }

    INLINE Vec3 Vec3::Cross(const Vec3 &vec1, const Vec3 &vec2)
    {
        return {
            vec1.Y() * vec2.Z() - vec2.Y() * vec1.Z(),
            vec1.Z() * vec2.X() - vec2.Z() * vec1.X(),
            vec1.X() * vec2.Y() - vec2.X() * vec1.Y()
        };
    }

    INLINE float Vec3::Distance(const Vec3 &other) const
    {
        return Distance(*this, other);
    }

    INLINE float Vec3::Distance(const Vec3 &vec1, const Vec3 &vec2)
    {
        return sqrtf(DistanceSquared(vec1, vec2));
    }

    INLINE float Vec3::DistanceSquared(const Vec3 &other) const
    {
        return DistanceSquared(*this, other);
    }

    INLINE float Vec3::DistanceSquared(const Vec3 &vec1, const Vec3 &vec2)
    {
        return (vec2 - vec1).LengthSquared();
    }

    INLINE float Vec3::Dot(const Vec3 &other) const
    {
        return Dot(*this, other);
    }

    INLINE float Vec3::Dot(const Vec3 &vec1, const Vec3 &vec2)
    {
        return vec1.X() * vec2.X() + vec1.Y() * vec2.Y() + vec1.Z() * vec2.Z();
    }

    INLINE float Vec3::Length(void) const
    {
        return Length(*this);
    }

    INLINE float Vec3::Length(const Vec3 &vec)
    {
        return sqrtf(LengthSquared(vec));
    }

    INLINE float Vec3::LengthSquared(void) const
    {
        return LengthSquared(*this);
    }

    INLINE float Vec3::LengthSquared(const Vec3 &vec)
    {
        return Dot(vec, vec);
    }

    INLINE void Vec3::Max(const Vec3 &other)
    {
        x = Math::Max(x, other.X());
        y = Math::Max(y, other.Y());
        z = Math::Max(z, other.Z());
    }

    INLINE Vec3 Vec3::Max(const Vec3 &vec1, const Vec3 &vec2)
    {
        return {
            Math::Max(vec1.X(), vec2.X()),
            Math::Max(vec1.Y(), vec2.Y()),
            Math::Max(vec1.Z(), vec2.Z())
        };
    }

    INLINE void Vec3::Min(const Vec3 &other)
    {
        x = Math::Min(x, other.X());
        y = Math::Min(y, other.Y());
        z = Math::Min(z, other.Z());
    }

    INLINE Vec3 Vec3::Min(const Vec3 &vec1, const Vec3 &vec2)
    {
        return {
            Math::Min(vec1.X(), vec2.X()),
            Math::Min(vec1.Y(), vec2.Y()),
            Math::Min(vec1.Z(), vec2.Z())
        };
    }

    INLINE void Vec3::Normalize(void)
    {
        *this /= Length(*this);
    }

    INLINE Vec3 Vec3::Normalize(const Vec3 &vec)
    {
        return vec / Length(vec);
    }

    INLINE void Vec3::Reflect(const Vec3 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(*this, normal);

        *this -= normal * k;
    }

    INLINE Vec3 Vec3::Reflect(const Vec3 &vec, const Vec3 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(vec, normal);

        return vec - (normal * k);
    }

    // Accessors
    INLINE float Vec3::X(void) const
    {
        return x;
    }

    INLINE float Vec3::Y(void) const
    {
        return y;
    }

    INLINE float Vec3::Z(void) const
    {
        return z;
    }

    INLINE float &Vec3::X(void)
    {
        return x;
    }

    INLINE float &Vec3::Y(void)
    {
        return y;
    }

    INLINE float &Vec3::Z(void)
    {
        return z;
    }

    INLINE float Vec3::operator[](uint index) const
    {
        return (&x)[index];
    }

    INLINE float &Vec3::operator[](uint index)
    {
        return (&x)[index];
    }

    INLINE const float* Vec3::GetFloatPtr(void) const
    {
        return &x;
    }

    // Operators
    INLINE bool Vec3::operator==(const Vec3 &rhs) const
    {
        return Math::IsEqual(x, rhs.X()) &&
               Math::IsEqual(y, rhs.Y()) &&
               Math::IsEqual(z, rhs.Z());
    }

    INLINE bool Vec3::operator!=(const Vec3 &rhs) const
    {
        return !(*this == rhs);
    }

    INLINE Vec3 Vec3::operator+(const Vec3 &rhs) const
    {
        return {
            x + rhs.X(),
            y + rhs.Y(),
            z + rhs.Z()
        };
    }

    INLINE Vec3 Vec3::operator-(void) const
    {
        return {
            -x, -y, -z
        };
    }

    INLINE Vec3 Vec3::operator-(const Vec3 &rhs) const
    {
        return {
            x - rhs.X(),
            y - rhs.Y(),
            z - rhs.Z()
        };
    }

    INLINE Vec3 Vec3::operator*(const Vec3 &rhs) const
    {
        return {
            x * rhs.X(),
            y * rhs.Y(),
            z * rhs.Z()
        };
    }

    INLINE Vec3 Vec3::operator*(float rhs) const
    {
        return {
            x * rhs,
            y * rhs,
            z * rhs
        };
    }

    INLINE Vec3 operator*(float lhs, const Vec3 &rhs)
    {
        return {
            rhs.X() * lhs,
            rhs.Y() * lhs,
            rhs.Z() * lhs
        };
    }

    INLINE Vec3 Vec3::operator/(const Vec3 &rhs) const
    {
        return {
            x / rhs.X(),
            y / rhs.Y(),
            z / rhs.Z()
        };
    }

    INLINE Vec3 Vec3::operator/(float rhs) const
    {
        float inv = 1.0f / rhs;

        return {
            x * inv,
            y * inv,
            z * inv
        };
    }

    INLINE const Vec3 &Vec3::operator=(const Vec3 &rhs)
    {
        x = rhs.X();
        y = rhs.Y();
        z = rhs.Z();

        return *this;
    }

    INLINE const Vec3 &Vec3::operator+=(const Vec3 &rhs)
    {
        x += rhs.X();
        y += rhs.Y();
        z += rhs.Z();

        return *this;
    }

    INLINE const Vec3 &Vec3::operator-=(const Vec3 &rhs)
    {
        x -= rhs.X();
        y -= rhs.Y();
        z -= rhs.Z();

        return *this;
    }

    INLINE const Vec3 &Vec3::operator*=(const Vec3 &rhs)
    {
        x *= rhs.X();
        y *= rhs.Y();
        z *= rhs.Z();

        return *this;
    }

    INLINE const Vec3 &Vec3::operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;

        return *this;
    }

    INLINE const Vec3 &Vec3::operator/=(const Vec3 &rhs)
    {
        x /= rhs.X();
        y /= rhs.Y();
        z /= rhs.Z();

        return *this;
    }

    INLINE const Vec3 &Vec3::operator/=(float rhs)
    {
        float inv = 1.0f / rhs;

        x *= inv;
        y *= inv;
        z *= inv;

        return *this;
    }
}