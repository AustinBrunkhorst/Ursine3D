#include <math.h>
#include "UrsineMath.h"

namespace Ursine
{
#ifdef USE_SSE
    INLINE SVec3::SVec3(const SIMDvec &value)
        : m128(value) { }
#endif

    INLINE SVec3::SVec3(float X, float Y, float Z)
            : x(X)
            , y(Y)
            , z(Z)
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }

    INLINE SVec3::SVec3(float value)
            : x(value)
            , y(value)
            , z(value)
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }

    INLINE SVec3::SVec3(const SVec3 &value)
            : x(value.X())
            , y(value.Y())
            , z(value.Z())
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }

    INLINE SVec3::SVec3(void)
            : x(0.0f)
            , y(0.0f)
            , z(0.0f)
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }

    // Properties
    INLINE const SVec3 &SVec3::Zero(void)
    {
        static const SVec3 zero;

        return zero;
    }

    INLINE const SVec3 &SVec3::One(void)
    {
        static const SVec3 one(1.0f);

        return one;
    }

    INLINE const SVec3 &SVec3::UnitX(void)
    {
        static const SVec3 unit_x(1.0f, 0.0f, 0.0f);

        return unit_x;
    }

    INLINE const SVec3 &SVec3::UnitY(void)
    {
        static const SVec3 unit_y(0.0f, 1.0f, 0.0f);

        return unit_y;
    }

    INLINE const SVec3 &SVec3::UnitZ(void)
    {
        static const SVec3 unit_z(0.0f, 0.0f, 1.0f);

        return unit_z;
    }

    // Public Methods
    INLINE void SVec3::Set(float X, float Y, float Z)
    {
#ifdef USE_SSE
        m128 = _mm_set_ps(0.0f, Z, Y, X);
#else
        x = X;
        y = Y;
        z = Z;
#endif
    }

    INLINE void SVec3::Clamp(const SVec3 &min, const SVec3 &max)
    {
        *this = Max(Min(*this, max), min);
    }

    INLINE SVec3 SVec3::Clamp(const SVec3 &value, const SVec3 &min, const SVec3 &max)
    {
        return Max(Min(value, max), min);
    }

    INLINE void SVec3::Cross(const SVec3 &other)
    {
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

        x = y          * other.Z() - other.Y() * z;
        y = z          * other.X() - other.Z() * original_x;
        z = original_x * other.Y() - other.X() * original_y;
#endif
    }

    INLINE float SVec3::TripleProduct(const SVec3& a, const SVec3& b, const SVec3& c)
    {
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
        return( (a.X() * (b.Y() * c.Z() - b.Z() * c.Y())) +
                (a.Y() * (b.Z() * c.X() - b.X() * c.Z())) +
                (a.Z() * (b.X() * c.Y() - b.Y() * c.X())) );
#endif
    }

    INLINE SVec3 SVec3::Cross(const SVec3 &vec1, const SVec3 &vec2)
    {
#ifdef USE_SSE
        auto t = _mm_shuffle_ps(vec1.m128, vec1.m128, SHUFFLE(1, 2, 0, 3)); // Y, Z, X, 0
        auto v = _mm_shuffle_ps(vec2.m128, vec2.m128, SHUFFLE(1, 2, 0, 3)); // Y, Z, X, 0

        v = _mm_mul_ps(v, vec1.m128);
        t = _mm_mul_ps(t, vec2.m128);
        v = _mm_sub_ps(v, t);

        return SVec3(_mm_shuffle_ps(v, v, SHUFFLE(1, 2, 0, 3)));
#else
        return {
            vec1.Y() * vec2.Z() - vec2.Y() * vec1.Z(),
            vec1.Z() * vec2.X() - vec2.Z() * vec1.X(),
            vec1.X() * vec2.Y() - vec2.X() * vec1.Y()
        };
#endif
    }

    INLINE float SVec3::Distance(const SVec3 &other) const
    {
        return Distance(*this, other);
    }

    INLINE float SVec3::Distance(const SVec3 &vec1, const SVec3 &vec2)
    {
        return sqrtf(DistanceSquared(vec1, vec2));
    }

    INLINE float SVec3::DistanceSquared(const SVec3 &other) const
    {
        return DistanceSquared(*this, other);
    }

    INLINE float SVec3::DistanceSquared(const SVec3 &vec1, const SVec3 &vec2)
    {
        return (vec2 - vec1).LengthSquared();
    }

    INLINE float SVec3::Dot(const SVec3 &other) const
    {
        return Dot(*this, other);
    }

    INLINE float SVec3::Dot(const SVec3 &vec1, const SVec3 &vec2)
    {
#ifdef USE_SSE
        auto v = _mm_mul_ps(vec1.m128, vec2.m128);
        auto z = _mm_movehl_ps(v, v);
        auto y = _mm_shuffle_ps(v, v, 0x55);

        v = _mm_add_ss(v, y);
        v = _mm_add_ss(v, z);

        return _mm_cvtss_f32(v);

        _mm_dp_ps(z, y, 0x55);
#else
        return vec1.X() * vec2.X() + vec1.Y() * vec2.Y() + vec1.Z() * vec2.Z();
#endif
    }

    INLINE float SVec3::Length(void) const
    {
        return Length(*this);
    }

    INLINE float SVec3::Length(const SVec3 &vec)
    {
        return sqrtf(LengthSquared(vec));
    }

    INLINE float SVec3::LengthSquared(void) const
    {
        return LengthSquared(*this);
    }

    INLINE float SVec3::LengthSquared(const SVec3 &vec)
    {
        return Dot(vec, vec);
    }

    INLINE void SVec3::Max(const SVec3 &other)
    {
#ifdef USE_SSE
        m128 = _mm_max_ps(m128, other.m128);
#else
        x = Math::Max(x, other.X());
        y = Math::Max(y, other.Y());
        z = Math::Max(z, other.Z());
#endif
    }

    INLINE SVec3 SVec3::Max(const SVec3 &vec1, const SVec3 &vec2)
    {
#ifdef USE_SSE
        return SVec3(_mm_max_ps(vec1.m128, vec2.m128));
#else
        return {
            Math::Max(vec1.X(), vec2.X()),
            Math::Max(vec1.Y(), vec2.Y()),
            Math::Max(vec1.Z(), vec2.Z())
        };
#endif
    }

    INLINE void SVec3::Min(const SVec3 &other)
    {
#ifdef USE_SSE
        m128 = _mm_min_ps(m128, other.m128);
#else
        x = Math::Min(x, other.X());
        y = Math::Min(y, other.Y());
        z = Math::Min(z, other.Z());
#endif
    }

    INLINE SVec3 SVec3::Min(const SVec3 &vec1, const SVec3 &vec2)
    {
#ifdef USE_SSE
        return SVec3(_mm_min_ps(vec1.m128, vec2.m128));
#else
        return {
            Math::Min(vec1.X(), vec2.X()),
            Math::Min(vec1.Y(), vec2.Y()),
            Math::Min(vec1.Z(), vec2.Z())
        };
#endif
    }

    INLINE void SVec3::Normalize(void)
    {
        *this /= Length(*this);
    }

    INLINE SVec3 SVec3::Normalize(const SVec3 &vec)
    {
        return vec / Length(vec);
    }

	INLINE void SVec3::Orthonormalize(SVec3 &vec1, SVec3 &vec2, SVec3 &vec3)
    {
		vec1.Normalize();
		vec2 -= ProjectToNorm(vec2, vec1);

		vec2.Normalize();
		vec3 -= ProjectToNorm(vec3, vec1);
		vec3 -= ProjectToNorm(vec3, vec2);

		vec3.Normalize();
    }

    INLINE void SVec3::Reflect(const SVec3 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(*this, normal);

        *this -= normal * k;
    }

    INLINE SVec3 SVec3::Reflect(const SVec3 &vec, const SVec3 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(vec, normal);

        return vec - (normal * k);
    }

	INLINE void SVec3::ProjectToNorm(const SVec3 &normal)
    {
		// TODO: assume(IsNormalized())
		*this = normal * Dot(normal);
    }

	INLINE SVec3 SVec3::ProjectToNorm(const SVec3 &direction, const SVec3 &normal)
    {
		// TODO: assume(direction.IsNormalized())
		return normal * direction.Dot(normal);
    }

    // Accessors
    INLINE float SVec3::X(void) const
    {
        return x;
    }

    INLINE float SVec3::Y(void) const
    {
        return y;
    }

    INLINE float SVec3::Z(void) const
    {
        return z;
    }

    INLINE float &SVec3::X(void)
    {
        return x;
    }

    INLINE float &SVec3::Y(void)
    {
        return y;
    }

    INLINE float &SVec3::Z(void)
    {
        return z;
    }

    INLINE float SVec3::operator[](uint index) const
    {
        return (&x)[index];
    }

    INLINE float &SVec3::operator[](uint index)
    {
        return (&x)[index];
    }

    INLINE const float* SVec3::GetFloatPtr(void) const
    {
        return &x;
    }

    // Operators
    INLINE bool SVec3::operator==(const SVec3 &rhs) const
    {
#ifdef USE_SSE
        return (0xf == _mm_movemask_ps(_mm_cmpeq_ps(m128, rhs.m128)));
#else
        return Math::IsEqual(x, rhs.X()) &&
               Math::IsEqual(y, rhs.Y()) &&
               Math::IsEqual(z, rhs.Z());
#endif
    }

    INLINE bool SVec3::operator!=(const SVec3 &rhs) const
    {
        return !(*this == rhs);
    }

    INLINE SVec3 SVec3::operator+(const SVec3 &rhs) const
    {
#ifdef USE_SSE
        return SVec3(_mm_add_ps(m128, rhs.m128));
#else
        return {
            x + rhs.X(),
            y + rhs.Y(),
            z + rhs.Z()
        };
#endif
    }

    INLINE SVec3 SVec3::operator-(void) const
    {
#ifdef USE_SSE
        // flip the sign bit (not safe with NaNs)
        return SVec3(_mm_xor_ps(m128, _mm_set1_ps(-0.0f)));
#else
        return {
            -x, -y, -z
        };
#endif
    }

    INLINE SVec3 SVec3::operator-(const SVec3 &rhs) const
    {
#ifdef USE_SSE
        return SVec3(_mm_sub_ps(m128, rhs.m128));
#else
        return {
            x - rhs.X(),
            y - rhs.Y(),
            z - rhs.Z()
        };
#endif
    }

    INLINE SVec3 SVec3::operator*(const SVec3 &rhs) const
    {
#ifdef USE_SSE
        return SVec3(_mm_mul_ps(m128, rhs.m128));
#else
        return {
            x * rhs.X(),
            y * rhs.Y(),
            z * rhs.Z()
        };
#endif
    }

    INLINE SVec3 SVec3::operator*(float rhs) const
    {
#ifdef USE_SSE
        return SVec3(_mm_mul_ps(m128, _mm_set1_ps(rhs)));
#else
        return {
            x * rhs,
            y * rhs,
            z * rhs
        };
#endif
    }

    INLINE SVec3 operator*(float lhs, const SVec3 &rhs)
    {
#ifdef USE_SSE
        return SVec3(_mm_mul_ps(_mm_set1_ps(lhs), rhs.m128));
#else
        return {
            rhs.X() * lhs,
            rhs.Y() * lhs,
            rhs.Z() * lhs
        };
#endif
    }

    INLINE SVec3 SVec3::operator/(const SVec3 &rhs) const
    {
#ifdef USE_SSE
        return SVec3(_mm_and_ps(_mm_div_ps(m128, rhs.m128), FFF0Mask));
#else
        return {
            x / rhs.X(),
            y / rhs.Y(),
            z / rhs.Z()
        };
#endif
    }

    INLINE SVec3 SVec3::operator/(float rhs) const
    {
        float inv = 1.0f / rhs;

#ifdef USE_SSE
        return SVec3(_mm_mul_ps(m128, _mm_set1_ps(inv)));
#else
        return {
            x * inv,
            y * inv,
            z * inv
        };
#endif
    }

    INLINE const SVec3 &SVec3::operator=(const SVec3 &rhs)
    {
#ifdef USE_SSE
        m128 = rhs.m128;
#else
        x = rhs.X();
        y = rhs.Y();
        z = rhs.Z();
#endif
        return *this;
    }

    INLINE const SVec3 &SVec3::operator+=(const SVec3 &rhs)
    {
#ifdef USE_SSE
        m128 = _mm_add_ps(m128, rhs.m128);
#else
        x += rhs.X();
        y += rhs.Y();
        z += rhs.Z();
#endif

        return *this;
    }

    INLINE const SVec3 &SVec3::operator-=(const SVec3 &rhs)
    {
#ifdef USE_SSE
        m128 = _mm_sub_ps(m128, rhs.m128);
#else
        x -= rhs.X();
        y -= rhs.Y();
        z -= rhs.Z();
#endif

        return *this;
    }

    INLINE const SVec3 &SVec3::operator*=(const SVec3 &rhs)
    {
#ifdef USE_SSE
        m128 = _mm_mul_ps(m128, rhs.m128);
#else
        x *= rhs.X();
        y *= rhs.Y();
        z *= rhs.Z();
#endif

        return *this;
    }

    INLINE const SVec3 &SVec3::operator*=(float rhs)
    {
#ifdef USE_SSE
        m128 = _mm_mul_ps(m128, _mm_set1_ps(rhs));
#else
        x *= rhs;
        y *= rhs;
        z *= rhs;
#endif

        return *this;
    }

    INLINE const SVec3 &SVec3::operator/=(const SVec3 &rhs)
    {
#ifdef USE_SSE
        m128 = _mm_and_ps(_mm_div_ps(m128, rhs.m128), FFF0Mask);
#else
        x /= rhs.X();
        y /= rhs.Y();
        z /= rhs.Z();
#endif

        return *this;
    }

    INLINE const SVec3 &SVec3::operator/=(float rhs)
    {
        float inv = 1.0f / rhs;

#ifdef USE_SSE
        m128 = _mm_mul_ps(m128, _mm_set1_ps(inv));
#else
        x *= inv;
        y *= inv;
        z *= inv;
#endif

        return *this;
    }
}