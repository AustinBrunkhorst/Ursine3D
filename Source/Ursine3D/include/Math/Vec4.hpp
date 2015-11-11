#include <math.h>
#include "UrsineMath.h"

namespace ursine
{
	INLINE Vec4::Vec4(void)
		: m_x( 0.0f )
		, m_y( 0.0f )
		, m_z( 0.0f )
		, m_w( 0.0f )
	{
	}

	INLINE Vec4::Vec4(const Vec4 &value)
		: m_x( value.X( ) )
		, m_y( value.Y( ) )
		, m_z( value.Z( ) )
		, m_w( value.m_w )
	{
	}

	INLINE Vec4::Vec4(float value)
		: m_x( value )
		, m_y( value )
		, m_z( value )
		, m_w( value )
	{
	}

	INLINE Vec4::Vec4(float X, float Y, float Z, float W)
		: m_x( X )
		, m_y( Y )
		, m_z( Z )
		, m_w( W )
	{
	}

	// Properties
	INLINE const Vec4 &Vec4::Zero(void)
	{
		static const Vec4 zero;

		return zero;
	}

	INLINE const Vec4 &Vec4::One(void)
	{
		static const Vec4 one( 1.0f );

		return one;
	}

	INLINE const Vec4 &Vec4::UnitX(void)
	{
		static const Vec4 unit_x( 1.0f, 0.0f, 0.0f, 0.0f );

		return unit_x;
	}

	INLINE const Vec4 &Vec4::UnitY(void)
	{
		static const Vec4 unit_y( 0.0f, 1.0f, 0.0f, 0.0f );

		return unit_y;
	}

	INLINE const Vec4 &Vec4::UnitZ(void)
	{
		static const Vec4 unit_z( 0.0f, 0.0f, 1.0f, 0.0f );

		return unit_z;
	}

	INLINE const Vec4 &Vec4::UnitW(void)
	{
		static const Vec4 unit_w( 0.0f, 0.0f, 0.0f, 1.0f );

		return unit_w;
	}

	// Public Methods
	INLINE void Vec4::Set(float X, float Y, float Z, float W)
	{
		m_x = X;
		m_y = Y;
		m_z = Z;
		m_w = W;
	}

	INLINE void Vec4::Clamp(const Vec4 &min, const Vec4 &max)
	{
		*this = Max( Min( *this, max ), min );
	}

	INLINE Vec4 Vec4::Clamp(const Vec4 &value, const Vec4 &min, const Vec4 &max)
	{
		return Max( Min( value, max ), min );
	}

	INLINE float Vec4::Distance(const Vec4 &other) const
	{
		return Distance( *this, other );
	}

	INLINE float Vec4::Distance(const Vec4 &vec1, const Vec4 &vec2)
	{
		return static_cast<float>( sqrt( DistanceSquared( vec1, vec2 ) ) );
	}

	INLINE float Vec4::DistanceSquared(const Vec4 &other) const
	{
		return DistanceSquared( *this, other );
	}

	INLINE float Vec4::DistanceSquared(const Vec4 &vec1, const Vec4 &vec2)
	{
		return ( vec1 - vec2 ).LengthSquared( );
	}

	INLINE float Vec4::Dot(const Vec4 &other) const
	{
		return Dot( *this, other );
	}

	INLINE float Vec4::Dot(const Vec4 &vec1, const Vec4 &vec2)
	{
		return vec1.X( ) * vec2.X( ) + vec1.Y( ) * vec2.Y( ) +
			vec1.Z( ) * vec2.Z( ) + vec1.m_w * vec2.m_w;
	}

	INLINE float Vec4::Length(void) const
	{
		return Length( *this );
	}

	INLINE float Vec4::Length(const Vec4 &vec)
	{
		return static_cast<float>( sqrt( LengthSquared( vec ) ) );
	}

	INLINE float Vec4::LengthSquared(void) const
	{
		return LengthSquared( *this );
	}

	INLINE float Vec4::LengthSquared(const Vec4 &vec)
	{
		return Dot( vec, vec );
	}

	INLINE void Vec4::Max(const Vec4 &other)
	{
		m_x = math::Max( m_x, other.X( ) );
		m_y = math::Max( m_y, other.Y( ) );
		m_z = math::Max( m_z, other.Z( ) );
		m_w = math::Max( m_w, other.m_w );
	}

	INLINE Vec4 Vec4::Max(const Vec4 &vec1, const Vec4 &vec2)
	{
		return {
			math::Max( vec1.X( ), vec2.X( ) ),
			math::Max( vec1.Y( ), vec2.Y( ) ),
			math::Max( vec1.Z( ), vec2.Z( ) ),
			math::Max( vec1.m_w, vec2.m_w )
		};
	}

	INLINE void Vec4::Min(const Vec4 &other)
	{
		m_x = math::Min( m_x, other.X( ) );
		m_y = math::Min( m_y, other.Y( ) );
		m_z = math::Min( m_z, other.Z( ) );
		m_w = math::Min( m_w, other.m_w );
	}

	INLINE Vec4 Vec4::Min(const Vec4 &vec1, const Vec4 &vec2)
	{
		return {
			math::Min( vec1.X( ), vec2.X( ) ),
			math::Min( vec1.Y( ), vec2.Y( ) ),
			math::Min( vec1.Z( ), vec2.Z( ) ),
			math::Min( vec1.m_w, vec2.m_w )
		};
	}

	INLINE void Vec4::Normalize(void)
	{
		*this /= Length( *this );
	}

	INLINE Vec4 Vec4::Normalize(const Vec4 &vec)
	{
		return vec / Length( vec );
	}

	INLINE void Vec4::Reflect(const Vec4 &normal)
	{
		// i - (2 * n * dot(i, n))
		float k = 2.0f * Dot( *this, normal );

		*this -= normal * k;
	}

	INLINE Vec4 Vec4::Reflect(const Vec4 &vec, const Vec4 &normal)
	{
		// i - (2 * n * dot(i, n))
		float k = 2.0f * Dot( vec, normal );

		return vec - ( normal * k );
	}

	// Accessors
	INLINE float Vec4::X(void) const
	{
		return m_x;
	}

	INLINE float Vec4::Y(void) const
	{
		return m_y;
	}

	INLINE float Vec4::Z(void) const
	{
		return m_z;
	}

	INLINE float Vec4::W(void) const
	{
		return m_w;
	}

	INLINE float &Vec4::X(void)
	{
		return m_x;
	}

	INLINE float &Vec4::Y(void)
	{
		return m_y;
	}

	INLINE float &Vec4::Z(void)
	{
		return m_z;
	}

	INLINE float &Vec4::W(void)
	{
		return m_w;
	}

    INLINE void Vec4::SetX(float x)
    {
        m_x = x;
    }

    INLINE void Vec4::SetY(float y)
    {
        m_y = y;
    }

    INLINE void Vec4::SetZ(float z)
    {
        m_z = z;
    }

    INLINE void Vec4::SetW(float w)
    {
        m_w = w;
    }

	INLINE float Vec4::operator[](uint index) const
	{
		return ( &m_x )[ index ];
	}

	INLINE float &Vec4::operator[](uint index)
	{
		return ( &m_x )[ index ];
	}

	INLINE const float *Vec4::GetFloatPtr(void) const
	{
		return &m_x;
	}

	// Operators
	INLINE bool Vec4::operator==(const Vec4 &rhs) const
	{
		return math::IsEqual( m_x, rhs.X( ) ) &&
			math::IsEqual( m_y, rhs.Y( ) ) &&
			math::IsEqual( m_z, rhs.Z( ) ) &&
			math::IsEqual( m_w, rhs.m_w );
	}

	INLINE bool Vec4::operator!=(const Vec4 &rhs) const
	{
		return !( *this == rhs );
	}

	INLINE Vec4 Vec4::operator+(const Vec4 &rhs) const
	{
		return {
			m_x + rhs.X( ),
			m_y + rhs.Y( ),
			m_z + rhs.Z( ),
			m_w + rhs.m_w
		};
	}

	INLINE Vec4 Vec4::operator-(void) const
	{
		return {
			-m_x, -m_y, -m_z, -m_w
		};
	}

	INLINE Vec4 Vec4::operator-(const Vec4 &rhs) const
	{
		return {
			m_x - rhs.X( ),
			m_y - rhs.Y( ),
			m_z - rhs.Z( ),
			m_w - rhs.m_w
		};
	}

	INLINE Vec4 Vec4::operator*(const Vec4 &rhs) const
	{
		return {
			m_x * rhs.X( ),
			m_y * rhs.Y( ),
			m_z * rhs.Z( ),
			m_w * rhs.m_w
		};
	}

	INLINE Vec4 Vec4::operator*(float rhs) const
	{
		return {
			m_x * rhs,
			m_y * rhs,
			m_z * rhs,
			m_w * rhs
		};
	}

	INLINE Vec4 operator*(float lhs, const Vec4 &rhs)
	{
		return {
			rhs.X( ) * lhs,
			rhs.Y( ) * lhs,
			rhs.Z( ) * lhs,
			rhs.m_w * lhs
		};
	}

	INLINE Vec4 Vec4::operator/(const Vec4 &rhs) const
	{
		return {
			m_x / rhs.X( ),
			m_y / rhs.Y( ),
			m_z / rhs.Z( ),
			m_w / rhs.m_w
		};
	}

	INLINE Vec4 Vec4::operator/(float rhs) const
	{
		float inv = 1.0f / rhs;

		return {
			m_x * inv,
			m_y * inv,
			m_z * inv,
			m_w * inv
		};
	}

	INLINE const Vec4 &Vec4::operator=(const Vec4 &rhs)
	{
		m_x = rhs.X( );
		m_y = rhs.Y( );
		m_z = rhs.Z( );
		m_w = rhs.m_w;

		return *this;
	}

	INLINE const Vec4 &Vec4::operator+=(const Vec4 &rhs)
	{
		m_x += rhs.X( );
		m_y += rhs.Y( );
		m_z += rhs.Z( );
		m_w += rhs.m_w;

		return *this;
	}

	INLINE const Vec4 &Vec4::operator-=(const Vec4 &rhs)
	{
		m_x -= rhs.X( );
		m_y -= rhs.Y( );
		m_z -= rhs.Z( );
		m_w -= rhs.m_w;

		return *this;
	}

	INLINE const Vec4 &Vec4::operator*=(const Vec4 &rhs)
	{
		m_x *= rhs.X( );
		m_y *= rhs.Y( );
		m_z *= rhs.Z( );
		m_w *= rhs.m_w;

		return *this;
	}

	INLINE const Vec4 &Vec4::operator*=(float rhs)
	{
		m_x *= rhs;
		m_y *= rhs;
		m_z *= rhs;
		m_w *= rhs;

		return *this;
	}

	INLINE const Vec4 &Vec4::operator/=(const Vec4 &rhs)
	{
		m_x /= rhs.X( );
		m_y /= rhs.Y( );
		m_z /= rhs.Z( );
		m_w /= rhs.m_w;

		return *this;
	}

	INLINE const Vec4 &Vec4::operator/=(float rhs)
	{
		float inv = 1.0f / rhs;

		m_x *= inv;
		m_y *= inv;
		m_z *= inv;
		m_w *= inv;

		return *this;
	}
}
