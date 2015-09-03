#include <math.h>
#include "UrsineMath.h"

namespace ursine
{
#ifdef USE_SSE
    INLINE SVec4::SVec4(const SIMDvec& value)
        : m_128(value) { }
#endif

    INLINE SVec4::SVec4(void)
        : m_x(0.0f)
        , m_y(0.0f)
        , m_z(0.0f)
        , m_w(0.0f) { }

    INLINE SVec4::SVec4(const SVec4 &value)
        : m_x(value.X())
        , m_y(value.Y())
        , m_z(value.Z())
        , m_w(value.m_w) { }

    INLINE SVec4::SVec4(float value)
        : m_x(value)
        , m_y(value)
        , m_z(value)
        , m_w(value) { }

    INLINE SVec4::SVec4(float X, float Y, float Z, float W)
        : m_x(X)
        , m_y(Y)
        , m_z(Z)
        , m_w(W) { }

    // Properties
    INLINE const SVec4 &SVec4::Zero(void)
    {
        static const SVec4 zero;

        return zero;
    }

    INLINE const SVec4 &SVec4::One(void)
    {
        static const SVec4 one(1.0f);

        return one;
    }

    INLINE const SVec4 &SVec4::UnitX(void)
    {
        static const SVec4 unit_x(1.0f, 0.0f, 0.0f, 0.0f);

        return unit_x;
    }

    INLINE const SVec4 &SVec4::UnitY(void)
    {
        static const SVec4 unit_y(0.0f, 1.0f, 0.0f, 0.0f);

        return unit_y;
    }

    INLINE const SVec4 &SVec4::UnitZ(void)
    {
        static const SVec4 unit_z(0.0f, 0.0f, 1.0f, 0.0f);

        return unit_z;
    }

    INLINE const SVec4 &SVec4::UnitW(void)
    {
        static const SVec4 unit_w(0.0f, 0.0f, 0.0f, 1.0f);

        return unit_w;
    }
    
    // Public Methods
    INLINE void SVec4::Set(float X, float Y, float Z, float W)
    {
#ifdef USE_SSE
        m_128 = _mm_set_ps(W, Z, Y, X);
#else
        m_x = X;
        m_y = Y;
        m_z = Z;
        m_w = W;
#endif
    }

    INLINE void SVec4::Clamp(const SVec4 &min, const SVec4 &max)
    {
        *this = Max(Min(*this, max), min);
    }

    INLINE SVec4 SVec4::Clamp(const SVec4 &value, const SVec4 &min, const SVec4 &max)
    {
        return Max(Min(value, max), min);
    }

    INLINE float SVec4::Distance(const SVec4 &other) const
    {
        return Distance(*this, other);
    }

    INLINE float SVec4::Distance(const SVec4 &vec1, const SVec4 &vec2)
    {
        return static_cast<float>(sqrt(DistanceSquared(vec1, vec2)));
    }

    INLINE float SVec4::DistanceSquared(const SVec4 &other) const
    {
        return DistanceSquared(*this, other);
    }

    INLINE float SVec4::DistanceSquared(const SVec4 &vec1, const SVec4 &vec2)
    {
        return (vec1 - vec2).LengthSquared();
    }

    INLINE float SVec4::Dot(const SVec4 &other) const
    {
        return Dot(*this, other);
    }

    INLINE float SVec4::Dot(const SVec4 &vec1, const SVec4 &vec2)
    {
#ifdef USE_SSE
        auto v = _mm_mul_ps(vec1.m_128, vec2.m_128);
        auto m_y = _mm_shuffle_ps(v, v, SHUFFLE(3, 2, 1, 0));

        v = _mm_add_ps(v, m_y);

        auto m_w = _mm_shuffle_ps(m_y, m_y, SHUFFLE(2, 3, 0, 1));

        SVec4 answer = SVec4(_mm_add_ps(m_y, m_w));
        return answer.X() + answer.Y();
#else
        return vec1.X() * vec2.X() + vec1.Y() * vec2.Y() +
               vec1.Z() * vec2.Z() + vec1.m_w * vec2.m_w;
#endif
    }

    INLINE float SVec4::Length(void) const
    {
        return Length(*this);
    }

    INLINE float SVec4::Length(const SVec4 &vec)
    {
        return static_cast<float>(sqrt(LengthSquared(vec)));
    }

    INLINE float SVec4::LengthSquared(void) const
    {
        return LengthSquared(*this);
    }

    INLINE float SVec4::LengthSquared(const SVec4 &vec)
    {
        return Dot(vec, vec);
    }

    INLINE void SVec4::Max(const SVec4 &other)
    {
#ifdef USE_SSE
        m_128 = _mm_max_ps(m_128, other.m_128);
#else
        m_x = math::Max(m_x, other.X());
        m_y = math::Max(m_y, other.Y());
        m_z = math::Max(m_z, other.Z());
        m_w = math::Max(m_w, other.W());
#endif
    }

    INLINE SVec4 SVec4::Max(const SVec4 &vec1, const SVec4 &vec2)
    {
#ifdef USE_SSE
        return SVec4(_mm_max_ps(vec1.m_128, vec2.m_128));
#else
        return{
            math::Max(vec1.X(), vec2.X()),
            math::Max(vec1.Y(), vec2.Y()),
            math::Max(vec1.Z(), vec2.Z()),
            math::Max(vec1.W(), vec2.W())
        };
#endif
    }

    INLINE void SVec4::Min(const SVec4 &other)
    {
#ifdef USE_SSE
        m_128 = _mm_min_ps(m_128, other.m_128);
#else
        m_x = math::Min(m_x, other.X());
        m_y = math::Min(m_y, other.Y());
        m_z = math::Min(m_z, other.Z());
        m_w = math::Min(m_w, other.W());
#endif
    }

    INLINE SVec4 SVec4::Min(const SVec4 &vec1, const SVec4 &vec2)
    {
#ifdef USE_SSE
        return SVec4(_mm_min_ps(vec1.m_128, vec2.m_128));
#else
        return{
            math::Min(vec1.X(), vec2.X()),
            math::Min(vec1.Y(), vec2.Y()),
            math::Min(vec1.Z(), vec2.Z()),
            math::Min(vec1.W(), vec2.W())
        };
#endif
    }

    INLINE void SVec4::Normalize(void)
    {
        *this /= Length(*this);
    }

    INLINE SVec4 SVec4::Normalize(const SVec4 &vec)
    {
        return vec / Length(vec);
    }

    INLINE void SVec4::Reflect(const SVec4 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(*this, normal);

        *this -= normal * k;
    }

    INLINE SVec4 SVec4::Reflect(const SVec4 &vec, const SVec4 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(vec, normal);

        return vec - (normal * k);
    }

    // Accessors
    INLINE float SVec4::X(void) const
    {
        return m_x;
    }

    INLINE float SVec4::Y(void) const
    {
        return m_y;
    }

    INLINE float SVec4::Z(void) const
    {
        return m_z;
    }

    INLINE float SVec4::W(void) const
    {
        return m_w;
    }

    INLINE float &SVec4::X(void)
    {
        return m_x;
    }

    INLINE float &SVec4::Y(void)
    {
        return m_y;
    }

    INLINE float &SVec4::Z(void)
    {
        return m_z;
    }

    INLINE float &SVec4::W(void)
    {
        return m_w;
    }

    INLINE float SVec4::operator[](uint index) const
    {
        return (&m_x)[index];
    }

    INLINE float &SVec4::operator[](uint index)
    {
        return (&m_x)[index];
    }

    INLINE const float* SVec4::GetFloatPtr(void) const
    {
        return &m_x;
    }

    // Operators
    INLINE bool SVec4::operator==(const SVec4 &rhs) const
    {
#ifdef USE_SSE
        SIMDvec dif = _mm_sub_ps(m_128, rhs.m_128);
        SIMDvec ep = _mm_set1_ps(math::Epsilon);
        SIMDvec neg_ep = _mm_set1_ps(-math::Epsilon);

        return (0xf == _mm_movemask_ps(_mm_and_ps(_mm_cmpgt_ps(ep, dif), _mm_cmplt_ps(neg_ep, dif))));
#else
        return math::IsEqual(m_x, rhs.X()) &&
               math::IsEqual(m_y, rhs.Y()) &&
               math::IsEqual(m_z, rhs.Z()) &&
               math::IsEqual(m_w, rhs.W());
#endif
    }

    INLINE bool SVec4::operator!=(const SVec4 &rhs) const
    {
        return !(*this == rhs);
    }

    INLINE SVec4 SVec4::operator+(const SVec4 &rhs) const
    {
#ifdef USE_SSE
        return SVec4(_mm_add_ps(m_128, rhs.m_128));
#else
        return {
            m_x + rhs.X(),
            m_y + rhs.Y(),
            m_z + rhs.Z(),
            m_w + rhs.W()
        };
#endif
    }

    INLINE SVec4 SVec4::operator-(void) const
    {
#ifdef USE_SSE
        // flip the sign bit (not safe with NaNs)
        return SVec4(_mm_xor_ps(m_128, _mm_set1_ps(-0.0f)));
#else
        return {
            -m_x, -m_y, -m_z, -m_w
        };
#endif
    }

    INLINE SVec4 SVec4::operator-(const SVec4 &rhs) const
    {
#ifdef USE_SSE
        return SVec4(_mm_sub_ps(m_128, rhs.m_128));
#else
        return {
            m_x - rhs.X(),
            m_y - rhs.Y(),
            m_z - rhs.Z(),
            m_w - rhs.W()
        };
#endif
    }

    INLINE SVec4 SVec4::operator*(const SVec4 &rhs) const
    {
#ifdef USE_SSE
        return SVec4(_mm_mul_ps(m_128, rhs.m_128));
#else
        return {
            m_x * rhs.X(),
            m_y * rhs.Y(),
            m_z * rhs.Z(),
            m_w * rhs.W()
        };
#endif
    }

    INLINE SVec4 SVec4::operator*(float rhs) const
    {
#ifdef USE_SSE
        return SVec4(_mm_mul_ps(m_128, _mm_set1_ps(rhs)));
#else
        return {
            m_x * rhs,
            m_y * rhs,
            m_z * rhs,
            m_w * rhs
        };
#endif
    }

    INLINE SVec4 operator*(float lhs, const SVec4 &rhs)
    {
#ifdef USE_SSE
        return SVec4(_mm_mul_ps(_mm_set1_ps(lhs), rhs.m_128));
#else
        return {
            rhs.X() * lhs,
            rhs.Y() * lhs,
            rhs.Z() * lhs,
            rhs.W() * lhs
        };
#endif
    }

    INLINE SVec4 SVec4::operator/(const SVec4 &rhs) const
    {
#ifdef USE_SSE
        return SVec4(_mm_div_ps(m_128, rhs.m_128));
#else
        return {
            m_x / rhs.X(),
            m_y / rhs.Y(),
            m_z / rhs.Z(),
            m_w / rhs.W()
        };
#endif
    }

    INLINE SVec4 SVec4::operator/(float rhs) const
    {
        float inv = 1.0f / rhs;

#ifdef USE_SSE
        return SVec4(_mm_mul_ps(m_128, _mm_set1_ps(inv)));
#else
        return {
            m_x * inv,
            m_y * inv,
            m_z * inv,
            m_w * inv
        };
#endif
    }

    INLINE const SVec4 &SVec4::operator=(const SVec4 &rhs)
    {
#ifdef USE_SSE
        m_128 = rhs.m_128;
#else
        m_x = rhs.X();
        m_y = rhs.Y();
        m_z = rhs.Z();
        m_w = rhs.W();
#endif

        return *this;
    }

    INLINE const SVec4 &SVec4::operator+=(const SVec4 &rhs)
    {
#ifdef USE_SSE
        m_128 = _mm_add_ps(m_128, rhs.m_128);
#else
        m_x += rhs.X();
        m_y += rhs.Y();
        m_z += rhs.Z();
        m_w += rhs.W();
#endif

        return *this;
    }

    INLINE const SVec4 &SVec4::operator-=(const SVec4 &rhs)
    {
#ifdef USE_SSE
        m_128 = _mm_sub_ps(m_128, rhs.m_128);
#else
        m_x -= rhs.X();
        m_y -= rhs.Y();
        m_z -= rhs.Z();
        m_w -= rhs.W();
#endif

        return *this;
    }

    INLINE const SVec4 &SVec4::operator*=(const SVec4 &rhs)
    {
#ifdef USE_SSE
        m_128 = _mm_mul_ps(m_128, rhs.m_128);
#else
        m_x *= rhs.X();
        m_y *= rhs.Y();
        m_z *= rhs.Z();
        m_w *= rhs.W();
#endif

        return *this;
    }

    INLINE const SVec4 &SVec4::operator*=(float rhs)
    {
#ifdef USE_SSE
        m_128 = _mm_mul_ps(m_128, _mm_set1_ps(rhs));
#else
        m_x *= rhs;
        m_y *= rhs;
        m_z *= rhs;
        m_w *= rhs;
#endif

        return *this;
    }

    INLINE const SVec4 &SVec4::operator/=(const SVec4 &rhs)
    {
#ifdef USE_SSE
        m_128 = _mm_div_ps(m_128, rhs.m_128);
#else
        m_x /= rhs.X();
        m_y /= rhs.Y();
        m_z /= rhs.Z();
        m_w /= rhs.W();
#endif

        return *this;
    }

    INLINE const SVec4 &SVec4::operator/=(float rhs)
    {
        float inv = 1.0f / rhs;

#ifdef USE_SSE
        m_128 = _mm_mul_ps(m_128, _mm_set1_ps(inv));
#else
        m_x *= inv;
        m_y *= inv;
        m_z *= inv;
        m_w *= inv;
#endif

        return *this;
    }
}
