#include "UrsineMath.h"

namespace Ursine
{
    // Constructors
    INLINE Vec2::Vec2(void)
        : x(0.0f)
        , y(0.0f) { }

    INLINE Vec2::Vec2(float X, float Y)
        : x(X)
        , y(Y) { }

    INLINE Vec2::Vec2(float value)
        : x(value)
        , y(value) { }

    INLINE Vec2::Vec2(const Vec2 &value)
        : x(value.X())
        , y(value.Y()) { }

    // Properties
    INLINE const Vec2 &Vec2::Zero(void)
    {
        static const Vec2 zero;

        return zero;
    }

    INLINE const Vec2 &Vec2::One(void)
    {
        static const Vec2 one(1.0f);

        return one;
    }

    INLINE const Vec2 &Vec2::UnitX(void)
    {
        static const Vec2 unit_x(1.0f, 0.0f);

        return unit_x;
    }

    INLINE const Vec2 &Vec2::UnitY(void)
    {
        static const Vec2 unit_y(0.0f, 1.0f);

        return unit_y;
    }

    // Accessors
    INLINE float Vec2::X(void) const
    {
        return x;
    }

    INLINE float Vec2::Y(void) const
    {
        return y;
    }

    INLINE float &Vec2::X(void)
    {
        return x;
    }

    INLINE float &Vec2::Y(void)
    {
        return y;
    }

    INLINE float Vec2::operator[](uint index) const
    {
        return (&x)[index];
    }

    INLINE float &Vec2::operator[](uint index)
    {
        return (&x)[index];
    }

    INLINE const float* Vec2::GetFloatPtr() const
    {
        return &x;
    }

    // Public Methods
    INLINE void Vec2::Set(float X, float Y)
    {
        x = X;
        y = Y;
    }

    INLINE void Vec2::Clamp(const Vec2 &min, const Vec2 &max)
    {
        *this = Max(Min(*this, max), min);
    }

    INLINE Vec2 Vec2::Clamp(const Vec2 &value, const Vec2 &min, const Vec2 &max)
    {
        return Max(Min(value, max), min);
    }

    INLINE float Vec2::Cross(const Vec2 &other) const
    {
        return Cross(*this, other);
    }

    INLINE void Vec2::Cross(float value)
    {
        float original_x = x;

        x = value * y;
        y = -value * original_x;
    }

    INLINE float Vec2::Cross(const Vec2 &vec1, const Vec2 &vec2)
    {
        return vec1.X() * vec2.Y() - vec1.Y() * vec2.X();
    }

    INLINE Vec2 Vec2::Cross(float value, const Vec2 &vec)
    {
        return{ -value * vec.Y(), value * vec.X() };
    }

	INLINE Vec2 Vec2::TripleProduct(const Vec2 &vec1, const Vec2 &vec2, const Vec2 &vec3)
	{
		// shortcut for triple cross product
		// (A x B) x C == B * A.Dot(C) - C * A.Dot(B)
		return (vec2 * Dot(vec1, vec3)) - (vec3 * Dot(vec1, vec2));
	}

    INLINE float Vec2::Distance(const Vec2 &other) const
    {
        return Distance(*this, other);
    }

    INLINE float Vec2::Distance(const Vec2 &vec1, const Vec2 &vec2)
    {
        return static_cast<float>(sqrt(DistanceSquared(vec1, vec2)));
    }

    INLINE float Vec2::DistanceSquared(const Vec2 &other) const
    {
        return DistanceSquared(*this, other);
    }

    INLINE float Vec2::DistanceSquared(const Vec2 &vec1, const Vec2 &vec2)
    {
        float a = vec1.X() - vec2.X(), b = vec1.Y() - vec2.Y();

        return a * a + b * b;
    }

    INLINE float Vec2::Dot(const Vec2 &other) const
    {
        return Dot(*this, other);
    }

    INLINE float Vec2::Dot(const Vec2 &vec1, const Vec2 &vec2)
    {
        return vec1.X() * vec2.X() + vec1.Y() * vec2.Y();
    }

    INLINE float Vec2::Length(void) const
    {
        return Length(*this);
    }

    INLINE float Vec2::Length(const Vec2 &vec)
    {
        float len_sq = LengthSquared(vec);
        return static_cast<float>(sqrt(len_sq));
    }

    INLINE float Vec2::LengthSquared(void) const
    {
        return LengthSquared(*this);
    }

    INLINE float Vec2::LengthSquared(const Vec2 &vec)
    {
        return vec.X() * vec.X() + vec.Y() * vec.Y();
    }

    INLINE float Vec2::Angle(void) const
    {
        return Angle(*this);
    }

    INLINE float Vec2::Angle(const Vec2 &vec)
    {
        float angle = atan2f(vec.Y(), vec.X());

        angle = Math::Wrap(angle, 0.0f, Math::PI_2);

        return angle;
    }

    INLINE Vec2 Vec2::AngleVec(float radians)
    {
        radians = Math::Wrap(radians, 0.0f, Math::PI_2);

        return{
            static_cast<float>(cos(radians)),
            static_cast<float>(sin(radians))
        };
    }

    INLINE void Vec2::Max(const Vec2 &other)
    {
        x = Math::Max(x, other.X());
        y = Math::Max(y, other.Y());
    }

    INLINE Vec2 Vec2::Max(const Vec2 &vec1, const Vec2 &vec2)
    {
        return{
            Math::Max(vec1.X(), vec2.X()),
            Math::Max(vec1.Y(), vec2.Y())
        };
    }

    INLINE void Vec2::Min(const Vec2 &other)
    {
        x = Math::Min(x, other.X());
        y = Math::Min(y, other.Y());
    }

    INLINE Vec2 Vec2::Min(const Vec2 &vec1, const Vec2 &vec2)
    {
        return{
            Math::Min(vec1.X(), vec2.X()),
            Math::Min(vec1.Y(), vec2.Y())
        };
    }

    INLINE void Vec2::Normalize(void)
    {
        float len = Length();

        if (len != 0.0f)
        {
            x /= len;
            y /= len;
        }
    }

    INLINE Vec2 Vec2::Normalize(const Vec2 &vec)
    {
        float len = vec.Length();

        if (len != 0.0f)
            return{ vec.X() / len, vec.Y() / len };

        return vec;
    }

    INLINE void Vec2::Reflect(const Vec2 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(*this, normal);

        *this -= normal * k;
    }

    INLINE Vec2 Vec2::Reflect(const Vec2 &vec, const Vec2 &normal)
    {
        // i - (2 * n * dot(i, n))
        float k = 2.0f * Dot(vec, normal);

        return vec - (normal * k);
    }

    INLINE bool Vec2::SameDirection(const Vec2 &other) const
    {
        return Dot(other) > 0.0f;
    }

    INLINE bool Vec2::SameDirection(const Vec2 &vec1, const Vec2 &vec2)
    {
        return Dot(vec1, vec2) > 0.0f;
    }

    INLINE bool Vec2::OppositeDirection(const Vec2 &other) const
    {
        return Dot(other) < 0.0f;
    }

    INLINE bool Vec2::OppositeDirection(const Vec2 &vec1, const Vec2 &vec2)
    {
        return Dot(vec1, vec2) < 0.0f;
    }

    // Operators
    INLINE bool Vec2::operator==(const Vec2 &rhs) const
    {
        return Math::IsEqual(x, rhs.X()) && Math::IsEqual(y, rhs.Y());
    }

    INLINE bool Vec2::operator!=(const Vec2 &rhs) const
    {
        return !(*this == rhs);
    }

    INLINE Vec2 Vec2::operator+(const Vec2 &rhs) const
    {
        return{ x + rhs.X(), y + rhs.Y() };
    }

    INLINE Vec2 Vec2::operator-(void) const
    {
        return{ -x, -y };
    }

    INLINE Vec2 Vec2::operator-(const Vec2 &rhs) const
    {
        return{ x - rhs.X(), y - rhs.Y() };
    }

    INLINE Vec2 Vec2::operator*(const Vec2 &rhs) const
    {
        return{ x * rhs.X(), y * rhs.Y() };
    }

    INLINE Vec2 Vec2::operator*(float rhs) const
    {
        return{ x * rhs, y * rhs };
    }

    INLINE Vec2 operator*(float lhs, const Vec2 &rhs)
    {
        return{ rhs.X() * lhs, rhs.Y() * lhs };
    }

    INLINE Vec2 Vec2::operator/(const Vec2 &rhs) const
    {
        return{ x / rhs.X(), y / rhs.Y() };
    }

    INLINE Vec2 Vec2::operator/(float rhs) const
    {
        float inverse = 1.0f / rhs;

        return{ x * inverse, y * inverse };
    }

    INLINE const Vec2 &Vec2::operator=(const Vec2 &rhs)
    {
        x = rhs.X();
        y = rhs.Y();

        return *this;
    }

    INLINE const Vec2 &Vec2::operator+=(const Vec2 &rhs)
    {
        x += rhs.X();
        y += rhs.Y();

        return *this;
    }

    INLINE const Vec2 &Vec2::operator-=(const Vec2 &rhs)
    {
        x -= rhs.X();
        y -= rhs.Y();

        return *this;
    }

    INLINE const Vec2 &Vec2::operator*=(const Vec2 &rhs)
    {
        x *= rhs.X();
        y *= rhs.Y();

        return *this;
    }

    INLINE const Vec2 &Vec2::operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;

        return *this;
    }

    INLINE const Vec2 &Vec2::operator/=(const Vec2 &rhs)
    {
        x /= rhs.X();
        y /= rhs.Y();

        return *this;
    }

    INLINE const Vec2 &Vec2::operator/=(float rhs)
    {
        float inverse = 1.0f / rhs;

        x *= inverse;
        y *= inverse;

        return *this;
    }
}
