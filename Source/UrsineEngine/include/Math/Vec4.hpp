#include <math.h>
#include "UrsineMath.h"

namespace Ursine
{
<<<<<<< HEAD
=======
#ifdef USE_SSE
    INLINE Vec4::Vec4(const SIMDvec& value)
        : m128(value) { }
#endif

>>>>>>> master
    INLINE Vec4::Vec4(void)
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(0.0f) { }

    INLINE Vec4::Vec4(const Vec4 &value)
<<<<<<< HEAD
        : x(value.X())
        , y(value.Y())
        , z(value.Z())
=======
        : x(value.x)
        , y(value.y)
        , z(value.z)
>>>>>>> master
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
<<<<<<< HEAD
=======
#ifdef USE_SSE
        m128 = _mm_set_ps(W, Z, Y, X);
#else
>>>>>>> master
        x = X;
        y = Y;
        z = Z;
        w = W;
<<<<<<< HEAD
=======
#endif
>>>>>>> master
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
<<<<<<< HEAD
        return vec1.X() * vec2.X() + vec1.Y() * vec2.Y() +
               vec1.Z() * vec2.Z() + vec1.w * vec2.w;
=======
#ifdef USE_SSE
        auto v = _mm_mul_ps(vec1.m128, vec2.m128);
        auto y = _mm_shuffle_ps(v, v, SHUFFLE(3, 2, 1, 0));

        v = _mm_add_ps(v, y);

        auto w = _mm_shuffle_ps(y, y, SHUFFLE(2, 3, 0, 1));

        Vec4 answer = Vec4(_mm_add_ps(y, w));
        return answer.x + answer.y;
#else
        return vec1.x * vec2.x + vec1.y * vec2.y +
               vec1.z * vec2.z + vec1.w * vec2.w;
#endif
>>>>>>> master
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
<<<<<<< HEAD
        x = Math::Max(x, other.X());
        y = Math::Max(y, other.Y());
        z = Math::Max(z, other.Z());
        w = Math::Max(w, other.w);
=======
#ifdef USE_SSE
        m128 = _mm_max_ps(m128, other.m128);
#else
        x = Math::Max(x, other.x);
        y = Math::Max(y, other.y);
        z = Math::Max(z, other.z);
        w = Math::Max(w, other.w);
#endif
>>>>>>> master
    }

    INLINE Vec4 Vec4::Max(const Vec4 &vec1, const Vec4 &vec2)
    {
<<<<<<< HEAD
        return{
            Math::Max(vec1.X(), vec2.X()),
            Math::Max(vec1.Y(), vec2.Y()),
            Math::Max(vec1.Z(), vec2.Z()),
            Math::Max(vec1.w, vec2.w)
        };
=======
#ifdef USE_SSE
        return Vec4(_mm_max_ps(vec1.m128, vec2.m128));
#else
        return{
            Math::Max(vec1.x, vec2.x),
            Math::Max(vec1.y, vec2.y),
            Math::Max(vec1.z, vec2.z),
            Math::Max(vec1.w, vec2.w)
        };
#endif
>>>>>>> master
    }

    INLINE void Vec4::Min(const Vec4 &other)
    {
<<<<<<< HEAD
        x = Math::Min(x, other.X());
        y = Math::Min(y, other.Y());
        z = Math::Min(z, other.Z());
        w = Math::Min(w, other.w);
=======
#ifdef USE_SSE
        m128 = _mm_min_ps(m128, other.m128);
#else
        x = Math::Min(x, other.x);
        y = Math::Min(y, other.y);
        z = Math::Min(z, other.z);
        w = Math::Min(w, other.w);
#endif
>>>>>>> master
    }

    INLINE Vec4 Vec4::Min(const Vec4 &vec1, const Vec4 &vec2)
    {
<<<<<<< HEAD
        return{
            Math::Min(vec1.X(), vec2.X()),
            Math::Min(vec1.Y(), vec2.Y()),
            Math::Min(vec1.Z(), vec2.Z()),
            Math::Min(vec1.w, vec2.w)
        };
=======
#ifdef USE_SSE
        return Vec4(_mm_min_ps(vec1.m128, vec2.m128));
#else
        return{
            Math::Min(vec1.x, vec2.x),
            Math::Min(vec1.y, vec2.y),
            Math::Min(vec1.z, vec2.z),
            Math::Min(vec1.w, vec2.w)
        };
#endif
>>>>>>> master
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
<<<<<<< HEAD
        return Math::IsEqual(x, rhs.X()) &&
               Math::IsEqual(y, rhs.Y()) &&
               Math::IsEqual(z, rhs.Z()) &&
               Math::IsEqual(w, rhs.w);
=======
#ifdef USE_SSE
        SIMDvec dif = _mm_sub_ps(m128, rhs.m128);
        SIMDvec ep = _mm_set1_ps(Math::Epsilon);
        SIMDvec neg_ep = _mm_set1_ps(-Math::Epsilon);

        return (0xf == _mm_movemask_ps(_mm_and_ps(_mm_cmpgt_ps(ep, dif), _mm_cmplt_ps(neg_ep, dif))));
#else
        return Math::IsEqual(x, rhs.x) &&
               Math::IsEqual(y, rhs.y) &&
               Math::IsEqual(z, rhs.z) &&
               Math::IsEqual(w, rhs.w);
#endif
>>>>>>> master
    }

    INLINE bool Vec4::operator!=(const Vec4 &rhs) const
    {
        return !(*this == rhs);
    }

    INLINE Vec4 Vec4::operator+(const Vec4 &rhs) const
    {
<<<<<<< HEAD
        return {
            x + rhs.X(),
            y + rhs.Y(),
            z + rhs.Z(),
            w + rhs.w
        };
=======
#ifdef USE_SSE
        return Vec4(_mm_add_ps(m128, rhs.m128));
#else
        return {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z,
            w + rhs.w
        };
#endif
>>>>>>> master
    }

    INLINE Vec4 Vec4::operator-(void) const
    {
<<<<<<< HEAD
        return {
            -x, -y, -z, -w
        };
=======
#ifdef USE_SSE
        // flip the sign bit (not safe with NaNs)
        return Vec4(_mm_xor_ps(m128, _mm_set1_ps(-0.0f)));
#else
        return {
            -x, -y, -z, -w
        };
#endif
>>>>>>> master
    }

    INLINE Vec4 Vec4::operator-(const Vec4 &rhs) const
    {
<<<<<<< HEAD
        return {
            x - rhs.X(),
            y - rhs.Y(),
            z - rhs.Z(),
            w - rhs.w
        };
=======
#ifdef USE_SSE
        return Vec4(_mm_sub_ps(m128, rhs.m128));
#else
        return {
            x - rhs.x,
            y - rhs.y,
            z - rhs.z,
            w - rhs.w
        };
#endif
>>>>>>> master
    }

    INLINE Vec4 Vec4::operator*(const Vec4 &rhs) const
    {
<<<<<<< HEAD
        return {
            x * rhs.X(),
            y * rhs.Y(),
            z * rhs.Z(),
            w * rhs.w
        };
=======
#ifdef USE_SSE
        return Vec4(_mm_mul_ps(m128, rhs.m128));
#else
        return {
            x * rhs.x,
            y * rhs.y,
            z * rhs.z,
            w * rhs.w
        };
#endif
>>>>>>> master
    }

    INLINE Vec4 Vec4::operator*(float rhs) const
    {
<<<<<<< HEAD
=======
#ifdef USE_SSE
        return Vec4(_mm_mul_ps(m128, _mm_set1_ps(rhs)));
#else
>>>>>>> master
        return {
            x * rhs,
            y * rhs,
            z * rhs,
            w * rhs
        };
<<<<<<< HEAD
=======
#endif
>>>>>>> master
    }

    INLINE Vec4 operator*(float lhs, const Vec4 &rhs)
    {
<<<<<<< HEAD
        return {
            rhs.X() * lhs,
            rhs.Y() * lhs,
            rhs.Z() * lhs,
            rhs.w * lhs
        };
=======
#ifdef USE_SSE
        return Vec4(_mm_mul_ps(_mm_set1_ps(lhs), rhs.m128));
#else
        return {
            rhs.x * lhs,
            rhs.y * lhs,
            rhs.z * lhs,
            rhs.w * lhs
        };
#endif
>>>>>>> master
    }

    INLINE Vec4 Vec4::operator/(const Vec4 &rhs) const
    {
<<<<<<< HEAD
        return {
            x / rhs.X(),
            y / rhs.Y(),
            z / rhs.Z(),
            w / rhs.w
        };
=======
#ifdef USE_SSE
        return Vec4(_mm_div_ps(m128, rhs.m128));
#else
        return {
            x / rhs.x,
            y / rhs.y,
            z / rhs.z,
            w / rhs.w
        };
#endif
>>>>>>> master
    }

    INLINE Vec4 Vec4::operator/(float rhs) const
    {
        float inv = 1.0f / rhs;

<<<<<<< HEAD
=======
#ifdef USE_SSE
        return Vec4(_mm_mul_ps(m128, _mm_set1_ps(inv)));
#else
>>>>>>> master
        return {
            x * inv,
            y * inv,
            z * inv,
            w * inv
        };
<<<<<<< HEAD
=======
#endif
>>>>>>> master
    }

    INLINE const Vec4 &Vec4::operator=(const Vec4 &rhs)
    {
<<<<<<< HEAD
        x = rhs.X();
        y = rhs.Y();
        z = rhs.Z();
        w = rhs.w;
=======
#ifdef USE_SSE
        m128 = rhs.m128;
#else
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec4 &Vec4::operator+=(const Vec4 &rhs)
    {
<<<<<<< HEAD
        x += rhs.X();
        y += rhs.Y();
        z += rhs.Z();
        w += rhs.w;
=======
#ifdef USE_SSE
        m128 = _mm_add_ps(m128, rhs.m128);
#else
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec4 &Vec4::operator-=(const Vec4 &rhs)
    {
<<<<<<< HEAD
        x -= rhs.X();
        y -= rhs.Y();
        z -= rhs.Z();
        w -= rhs.w;
=======
#ifdef USE_SSE
        m128 = _mm_sub_ps(m128, rhs.m128);
#else
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec4 &Vec4::operator*=(const Vec4 &rhs)
    {
<<<<<<< HEAD
        x *= rhs.X();
        y *= rhs.Y();
        z *= rhs.Z();
        w *= rhs.w;
=======
#ifdef USE_SSE
        m128 = _mm_mul_ps(m128, rhs.m128);
#else
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec4 &Vec4::operator*=(float rhs)
    {
<<<<<<< HEAD
=======
#ifdef USE_SSE
        m128 = _mm_mul_ps(m128, _mm_set1_ps(rhs));
#else
>>>>>>> master
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
<<<<<<< HEAD
=======
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec4 &Vec4::operator/=(const Vec4 &rhs)
    {
<<<<<<< HEAD
        x /= rhs.X();
        y /= rhs.Y();
        z /= rhs.Z();
        w /= rhs.w;
=======
#ifdef USE_SSE
        m128 = _mm_div_ps(m128, rhs.m128);
#else
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;
#endif
>>>>>>> master

        return *this;
    }

    INLINE const Vec4 &Vec4::operator/=(float rhs)
    {
        float inv = 1.0f / rhs;

<<<<<<< HEAD
=======
#ifdef USE_SSE
        m128 = _mm_mul_ps(m128, _mm_set1_ps(inv));
#else
>>>>>>> master
        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;
<<<<<<< HEAD
=======
#endif
>>>>>>> master

        return *this;
    }
}
