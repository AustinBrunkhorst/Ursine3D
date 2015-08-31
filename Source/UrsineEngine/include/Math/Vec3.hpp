#include <math.h>
#include "UrsineMath.h"

namespace Ursine
{
<<<<<<< HEAD
=======
#ifdef USE_SSE
    INLINE Vec3::Vec3(const SIMDvec &value)
        : m128(value) { }
#endif

>>>>>>> master
    INLINE Vec3::Vec3(float X, float Y, float Z)
            : x(X)
            , y(Y)
            , z(Z)
<<<<<<< HEAD
    { }
=======
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }
>>>>>>> master

    INLINE Vec3::Vec3(float value)
            : x(value)
            , y(value)
            , z(value)
<<<<<<< HEAD
    { }

    INLINE Vec3::Vec3(const Vec3 &value)
            : x(value.X())
            , y(value.Y())
            , z(value.Z())
    { }
=======
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }

    INLINE Vec3::Vec3(const Vec3 &value)
            : x(value.x)
            , y(value.y)
            , z(value.z)
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }
>>>>>>> master

    INLINE Vec3::Vec3(void)
            : x(0.0f)
            , y(0.0f)
            , z(0.0f)
<<<<<<< HEAD
    { }
=======
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }
>>>>>>> master

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
<<<<<<< HEAD
        x = X;
        y = Y;
        z = Z;
=======
#ifdef USE_SSE
        m128 = _mm_set_ps(0.0f, Z, Y, X);
#else
        x = X;
        y = Y;
        z = Z;
#endif
>>>>>>> master
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
<<<<<<< HEAD
        float original_x = x;
        float original_y = y;

        x = y          * other.Z() - other.Y() * z;
        y = z          * other.X() - other.Z() * original_x;
        z = original_x * other.Y() - other.X() * original_y;
=======
#ifdef USE_SSE
        auto t = _mm_shuffle_ps(m128, m128, SHUFFLE(1, 2, 0, 3)); // Y, Z, X, 0
        auto v = _mm_shuffle_ps(other.m128, other.m128, SHUFFLE(1, 2, 0, 3)); // Y, Z, X, 0

        v = _mm_mul_ps(v, m128);
        t = _mm_mul_ps(t, other.m128);
        v = _mm_sub_ps(v, t);

        m128 = _mm_shuffle_ps(v, v, SHUFFLE(1, 2, 0, 3));
#else
        float original_x = x;
        float original_y = y;

        x = y          * other.z - other.y * z;
        y = z          * other.x - other.z * original_x;
        z = original_x * other.y - other.x * original_y;
#endif
>>>>>>> master
    }

    INLINE float Vec3::TripleProduct(const Vec3& a, const Vec3& b, const Vec3& c)
    {
<<<<<<< HEAD
        return( (a.X() * (b.Y() * c.Z() - b.Z() * c.Y())) +
                (a.Y() * (b.Z() * c.X() - b.X() * c.Z())) +
                (a.Z() * (b.X() * c.Y() - b.Y() * c.X())) );
=======
#ifdef USE_SSE
        // cross
        auto t = _mm_shuffle_ps(b.m128, b.m128, SHUFFLE(1, 2, 0, 3)); // Y, Z, X, 0
        auto v = _mm_shuffle_ps(c.m128, c.m128, SHUFFLE(1, 2, 0, 3)); // Y, Z, X, 0

        v = _mm_mul_ps(v, b.m128);
        t = _mm_mul_ps(t, c.m128);
        v = _mm_sub_ps(v, t);

        v = _mm_shuffle_ps(v, v, SHUFFLE(1, 2, 0, 3));

        // dot
        v = _mm_mul_ps(v, a.m128);
        auto z = _mm_movehl_ps(v, v);
        auto y = _mm_shuffle_ps(v, v, 0x55);
        v = _mm_add_ss(v, y);
        v = _mm_add_ss(v, z);
        return _mm_cvtss_f32(v);
#else
        return( (a.x * (b.y * c.z - b.z * c.y)) +
                (a.y * (b.z * c.x - b.x * c.z)) +
                (a.z * (b.x * c.y - b.y * c.x)) );
#endif
>>>>>>> master
    }

    INLINE Vec3 Vec3::Cross(const Vec3 &vec1, const Vec3 &vec2)
    {
<<<<<<< HEAD
        return {
            vec1.Y() * vec2.Z() - vec2.Y() * vec1.Z(),
            vec1.Z() * vec2.X() - vec2.Z() * vec1.X(),
            vec1.X() * vec2.Y() - vec2.X() * vec1.Y()
        };
=======
#ifdef USE_SSE
        auto t = _mm_shuffle_ps(vec1.m128, vec1.m128, SHUFFLE(1, 2, 0, 3)); // Y, Z, X, 0
        auto v = _mm_shuffle_ps(vec2.m128, vec2.m128, SHUFFLE(1, 2, 0, 3)); // Y, Z, X, 0

        v = _mm_mul_ps(v, vec1.m128);
        t = _mm_mul_ps(t, vec2.m128);
        v = _mm_sub_ps(v, t);

        return Vec3(_mm_shuffle_ps(v, v, SHUFFLE(1, 2, 0, 3)));
#else
        return {
            vec1.y * vec2.z - vec2.y * vec1.z,
            vec1.z * vec2.x - vec2.z * vec1.x,
            vec1.x * vec2.y - vec2.x * vec1.y
        };
#endif
>>>>>>> master
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
<<<<<<< HEAD
        return vec1.X() * vec2.X() + vec1.Y() * vec2.Y() + vec1.Z() * vec2.Z();
=======
#ifdef USE_SSE
        auto v = _mm_mul_ps(vec1.m128, vec2.m128);
        auto z = _mm_movehl_ps(v, v);
        auto y = _mm_shuffle_ps(v, v, 0x55);

        v = _mm_add_ss(v, y);
        v = _mm_add_ss(v, z);

        return _mm_cvtss_f32(v);

        _mm_dp_ps(z, y, 0x55);
#else
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
#endif
>>>>>>> master
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
<<<<<<< HEAD
        x = Math::Max(x, other.X());
        y = Math::Max(y, other.Y());
        z = Math::Max(z, other.Z());
=======
#ifdef USE_SSE
        m128 = _mm_max_ps(m128, other.m128);
#else
        x = Math::Max(x, other.x);
        y = Math::Max(y, other.y);
        z = Math::Max(z, other.z);
#endif
>>>>>>> master
    }

    INLINE Vec3 Vec3::Max(const Vec3 &vec1, const Vec3 &vec2)
    {
<<<<<<< HEAD
        return {
            Math::Max(vec1.X(), vec2.X()),
            Math::Max(vec1.Y(), vec2.Y()),
            Math::Max(vec1.Z(), vec2.Z())
        };
=======
#ifdef USE_SSE
        return Vec3(_mm_max_ps(vec1.m128, vec2.m128));
#else
        return {
            Math::Max(vec1.x, vec2.x),
            Math::Max(vec1.y, vec2.y),
            Math::Max(vec1.z, vec2.z)
        };
#endif
>>>>>>> master
    }

    INLINE void Vec3::Min(const Vec3 &other)
    {
<<<<<<< HEAD
        x = Math::Min(x, other.X());
        y = Math::Min(y, other.Y());
        z = Math::Min(z, other.Z());
=======
#ifdef USE_SSE
        m128 = _mm_min_ps(m128, other.m128);
#else
        x = Math::Min(x, other.x);
        y = Math::Min(y, other.y);
        z = Math::Min(z, other.z);
#endif
>>>>>>> master
    }

    INLINE Vec3 Vec3::Min(const Vec3 &vec1, const Vec3 &vec2)
    {
<<<<<<< HEAD
        return {
            Math::Min(vec1.X(), vec2.X()),
            Math::Min(vec1.Y(), vec2.Y()),
            Math::Min(vec1.Z(), vec2.Z())
        };
=======
#ifdef USE_SSE
        return Vec3(_mm_min_ps(vec1.m128, vec2.m128));
#else
        return {
            Math::Min(vec1.x, vec2.x),
            Math::Min(vec1.y, vec2.y),
            Math::Min(vec1.z, vec2.z)
        };
#endif
>>>>>>> master
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
<<<<<<< HEAD
        return Math::IsEqual(x, rhs.X()) &&
               Math::IsEqual(y, rhs.Y()) &&
               Math::IsEqual(z, rhs.Z());
=======
#ifdef USE_SSE
        return (0xf == _mm_movemask_ps(_mm_cmpeq_ps(m128, rhs.m128)));
#else
        return Math::IsEqual(x, rhs.x) &&
               Math::IsEqual(y, rhs.y) &&
               Math::IsEqual(z, rhs.z);
#endif
>>>>>>> master
    }

    INLINE bool Vec3::operator!=(const Vec3 &rhs) const
    {
        return !(*this == rhs);
    }

    INLINE Vec3 Vec3::operator+(const Vec3 &rhs) const
    {
<<<<<<< HEAD
        return {
            x + rhs.X(),
            y + rhs.Y(),
            z + rhs.Z()
        };
=======
#ifdef USE_SSE
        return Vec3(_mm_add_ps(m128, rhs.m128));
#else
        return {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z
        };
#endif
>>>>>>> master
    }

    INLINE Vec3 Vec3::operator-(void) const
    {
<<<<<<< HEAD
        return {
            -x, -y, -z
        };
=======
#ifdef USE_SSE
        // flip the sign bit (not safe with NaNs)
        return Vec3(_mm_xor_ps(m128, _mm_set1_ps(-0.0f)));
#else
        return {
            -x, -y, -z
        };
#endif
>>>>>>> master
    }

    INLINE Vec3 Vec3::operator-(const Vec3 &rhs) const
    {
<<<<<<< HEAD
        return {
            x - rhs.X(),
            y - rhs.Y(),
            z - rhs.Z()
        };
=======
#ifdef USE_SSE
        return Vec3(_mm_sub_ps(m128, rhs.m128));
#else
        return {
            x - rhs.x,
            y - rhs.y,
            z - rhs.z
        };
#endif
>>>>>>> master
    }

    INLINE Vec3 Vec3::operator*(const Vec3 &rhs) const
    {
<<<<<<< HEAD
        return {
            x * rhs.X(),
            y * rhs.Y(),
            z * rhs.Z()
        };
=======
#ifdef USE_SSE
        return Vec3(_mm_mul_ps(m128, rhs.m128));
#else
        return {
            x * rhs.x,
            y * rhs.y,
            z * rhs.z
        };
#endif
>>>>>>> master
    }

    INLINE Vec3 Vec3::operator*(float rhs) const
    {
<<<<<<< HEAD
=======
#ifdef USE_SSE
        return Vec3(_mm_mul_ps(m128, _mm_set1_ps(rhs)));
#else
>>>>>>> master
        return {
            x * rhs,
            y * rhs,
            z * rhs
        };
<<<<<<< HEAD
=======
#endif
>>>>>>> master
    }

    INLINE Vec3 operator*(float lhs, const Vec3 &rhs)
    {
<<<<<<< HEAD
        return {
            rhs.X() * lhs,
            rhs.Y() * lhs,
            rhs.Z() * lhs
        };
=======
#ifdef USE_SSE
        return Vec3(_mm_mul_ps(_mm_set1_ps(lhs), rhs.m128));
#else
        return {
            rhs.x * lhs,
            rhs.y * lhs,
            rhs.z * lhs
        };
#endif
>>>>>>> master
    }

    INLINE Vec3 Vec3::operator/(const Vec3 &rhs) const
    {
<<<<<<< HEAD
        return {
            x / rhs.X(),
            y / rhs.Y(),
            z / rhs.Z()
        };
=======
#ifdef USE_SSE
        return Vec3(_mm_and_ps(_mm_div_ps(m128, rhs.m128), FFF0Mask));
#else
        return {
            x / rhs.x,
            y / rhs.y,
            z / rhs.z
        };
#endif
>>>>>>> master
    }

    INLINE Vec3 Vec3::operator/(float rhs) const
    {
        float inv = 1.0f / rhs;

<<<<<<< HEAD
=======
#ifdef USE_SSE
        return Vec3(_mm_mul_ps(m128, _mm_set1_ps(inv)));
#else
>>>>>>> master
        return {
            x * inv,
            y * inv,
            z * inv
        };
<<<<<<< HEAD
=======
#endif
>>>>>>> master
    }

    INLINE const Vec3 &Vec3::operator=(const Vec3 &rhs)
    {
<<<<<<< HEAD
        x = rhs.X();
        y = rhs.Y();
        z = rhs.Z();

=======
#ifdef USE_SSE
        m128 = rhs.m128;
#else
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
#endif
>>>>>>> master
        return *this;
    }

    INLINE const Vec3 &Vec3::operator+=(const Vec3 &rhs)
    {
<<<<<<< HEAD
        x += rhs.X();
        y += rhs.Y();
        z += rhs.Z();
=======
#ifdef USE_SSE
        m128 = _mm_add_ps(m128, rhs.m128);
#else
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec3 &Vec3::operator-=(const Vec3 &rhs)
    {
<<<<<<< HEAD
        x -= rhs.X();
        y -= rhs.Y();
        z -= rhs.Z();
=======
#ifdef USE_SSE
        m128 = _mm_sub_ps(m128, rhs.m128);
#else
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec3 &Vec3::operator*=(const Vec3 &rhs)
    {
<<<<<<< HEAD
        x *= rhs.X();
        y *= rhs.Y();
        z *= rhs.Z();
=======
#ifdef USE_SSE
        m128 = _mm_mul_ps(m128, rhs.m128);
#else
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec3 &Vec3::operator*=(float rhs)
    {
<<<<<<< HEAD
        x *= rhs;
        y *= rhs;
        z *= rhs;
=======
#ifdef USE_SSE
        m128 = _mm_mul_ps(m128, _mm_set1_ps(rhs));
#else
        x *= rhs;
        y *= rhs;
        z *= rhs;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec3 &Vec3::operator/=(const Vec3 &rhs)
    {
<<<<<<< HEAD
        x /= rhs.X();
        y /= rhs.Y();
        z /= rhs.Z();
=======
#ifdef USE_SSE
        m128 = _mm_and_ps(_mm_div_ps(m128, rhs.m128), FFF0Mask);
#else
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec3 &Vec3::operator/=(float rhs)
    {
        float inv = 1.0f / rhs;

<<<<<<< HEAD
        x *= inv;
        y *= inv;
        z *= inv;
=======
#ifdef USE_SSE
        m128 = _mm_mul_ps(m128, _mm_set1_ps(inv));
#else
        x *= inv;
        y *= inv;
        z *= inv;
#endif
>>>>>>> master

        return *this;
    }
}