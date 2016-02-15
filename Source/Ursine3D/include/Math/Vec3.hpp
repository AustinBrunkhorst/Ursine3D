#include <math.h>
#include "UrsineMath.h"

namespace ursine
{
	INLINE Vec3::Vec3(float X, float Y, float Z)
		: m_x( X )
		, m_y( Y )
		, m_z( Z )
	{
	}

	INLINE Vec3::Vec3(float value)
		: m_x( value )
		, m_y( value )
		, m_z( value )
	{
	}

	INLINE Vec3::Vec3(const Vec3 &value)
		: m_x( value.X( ) )
		, m_y( value.Y( ) )
		, m_z( value.Z( ) )
	{
	}

	INLINE Vec3::Vec3(void)
		: m_x( 0.0f )
		, m_y( 0.0f )
		, m_z( 0.0f )
	{
	}

	// Properties
	INLINE const Vec3 &Vec3::Zero(void)
	{
		static const Vec3 zero;

		return zero;
	}

	INLINE const Vec3 &Vec3::One(void)
	{
		static const Vec3 one( 1.0f );

		return one;
	}

	INLINE const Vec3 &Vec3::UnitX(void)
	{
		static const Vec3 unit_x( 1.0f, 0.0f, 0.0f );

		return unit_x;
	}

	INLINE const Vec3 &Vec3::UnitY(void)
	{
		static const Vec3 unit_y( 0.0f, 1.0f, 0.0f );

		return unit_y;
	}

	INLINE const Vec3 &Vec3::UnitZ(void)
	{
		static const Vec3 unit_z( 0.0f, 0.0f, 1.0f );

		return unit_z;
	}

	// Public Methods
	INLINE void Vec3::Set(float X, float Y, float Z)
	{
		m_x = X;
		m_y = Y;
		m_z = Z;
	}

	INLINE void Vec3::Clamp(const Vec3 &min, const Vec3 &max)
	{
		*this = Max( Min( *this, max ), min );
	}

	INLINE Vec3 Vec3::Clamp(const Vec3 &value, const Vec3 &min, const Vec3 &max)
	{
		return Max( Min( value, max ), min );
	}

	INLINE void Vec3::Cross(const Vec3 &other)
	{
		float original_x = m_x;
		float original_y = m_y;

		m_x = m_y * other.Z( ) - other.Y( ) * m_z;
		m_y = m_z * other.X( ) - other.Z( ) * original_x;
		m_z = original_x * other.Y( ) - other.X( ) * original_y;
	}

	INLINE float Vec3::TripleProduct(const Vec3 &a, const Vec3 &b, const Vec3 &c)
	{
		return( ( a.X( ) * ( b.Y( ) * c.Z( ) - b.Z( ) * c.Y( ) ) ) +
			( a.Y( ) * ( b.Z( ) * c.X( ) - b.X( ) * c.Z( ) ) ) +
			( a.Z( ) * ( b.X( ) * c.Y( ) - b.Y( ) * c.X( ) ) ) );
	}

	INLINE Vec3 Vec3::Cross(const Vec3 &vec1, const Vec3 &vec2)
	{
		return {
			vec1.Y( ) * vec2.Z( ) - vec2.Y( ) * vec1.Z( ),
			vec1.Z( ) * vec2.X( ) - vec2.Z( ) * vec1.X( ),
			vec1.X( ) * vec2.Y( ) - vec2.X( ) * vec1.Y( )
		};
	}

	INLINE float Vec3::Distance(const Vec3 &other) const
	{
		return Distance( *this, other );
	}

	INLINE float Vec3::Distance(const Vec3 &vec1, const Vec3 &vec2)
	{
		return sqrtf( DistanceSquared( vec1, vec2 ) );
	}

	INLINE float Vec3::DistanceSquared(const Vec3 &other) const
	{
		return DistanceSquared( *this, other );
	}

	INLINE float Vec3::DistanceSquared(const Vec3 &vec1, const Vec3 &vec2)
	{
		return ( vec2 - vec1 ).LengthSquared( );
	}

	INLINE float Vec3::Dot(const Vec3 &other) const
	{
		return Dot( *this, other );
	}

	INLINE float Vec3::Dot(const Vec3 &vec1, const Vec3 &vec2)
	{
		return vec1.X( ) * vec2.X( ) + vec1.Y( ) * vec2.Y( ) + vec1.Z( ) * vec2.Z( );
	}

	INLINE float Vec3::Length(void) const
	{
		return Length( *this );
	}

	INLINE float Vec3::Length(const Vec3 &vec)
	{
		return sqrtf( LengthSquared( vec ) );
	}

	INLINE float Vec3::LengthSquared(void) const
	{
		return LengthSquared( *this );
	}

	INLINE float Vec3::LengthSquared(const Vec3 &vec)
	{
		return Dot( vec, vec );
	}

	INLINE void Vec3::Max(const Vec3 &other)
	{
		m_x = math::Max( m_x, other.X( ) );
		m_y = math::Max( m_y, other.Y( ) );
		m_z = math::Max( m_z, other.Z( ) );
	}

	INLINE Vec3 Vec3::Max(const Vec3 &vec1, const Vec3 &vec2)
	{
		return {
			math::Max( vec1.X( ), vec2.X( ) ),
			math::Max( vec1.Y( ), vec2.Y( ) ),
			math::Max( vec1.Z( ), vec2.Z( ) )
		};
	}

	INLINE void Vec3::Min(const Vec3 &other)
	{
		m_x = math::Min( m_x, other.X( ) );
		m_y = math::Min( m_y, other.Y( ) );
		m_z = math::Min( m_z, other.Z( ) );
	}

	INLINE Vec3 Vec3::Min(const Vec3 &vec1, const Vec3 &vec2)
	{
		return {
			math::Min( vec1.X( ), vec2.X( ) ),
			math::Min( vec1.Y( ), vec2.Y( ) ),
			math::Min( vec1.Z( ), vec2.Z( ) )
		};
	}

	INLINE void Vec3::Normalize(void)
	{
		*this /= Length( *this );
	}

	INLINE Vec3 Vec3::Normalize(const Vec3 &vec)
	{
		return vec / Length( vec );
	}

	INLINE void Vec3::Reflect(const Vec3 &normal)
	{
		// i - (2 * n * dot(i, n))
		float k = 2.0f * Dot( *this, normal );

		*this -= normal * k;
	}

	INLINE Vec3 Vec3::Reflect(const Vec3 &vec, const Vec3 &normal)
	{
		// i - (2 * n * dot(i, n))
		float k = 2.0f * Dot( vec, normal );

		return vec - ( normal * k );
	}

    INLINE void Vec3::GenerateOrthogonalVectors(Vec3 &u, Vec3 &v)
    {
        Normalize();
        u = Vec3::Cross(*this, *this == Vec3::UnitY() ? Vec3::UnitX() : Vec3::UnitY());
        v = Vec3::Cross(*this, u);
    }

    INLINE void Vec3::GenerateOrthogonalVectors(const Vec3 &inputVec, Vec3 &u, Vec3 &v)
    {
        auto normal = Normalize(inputVec);
        u = Vec3::Cross(normal, normal == Vec3::UnitY() ? Vec3::UnitX() : Vec3::UnitY());
        v = Vec3::Cross(normal, u);
    }

	// Accessors
	INLINE float Vec3::X(void) const
	{
		return m_x;
	}

	INLINE float Vec3::Y(void) const
	{
		return m_y;
	}

	INLINE float Vec3::Z(void) const
	{
		return m_z;
	}

	INLINE float &Vec3::X(void)
	{
		return m_x;
	}

	INLINE float &Vec3::Y(void)
	{
		return m_y;
	}

	INLINE float &Vec3::Z(void)
	{
		return m_z;
	}

    INLINE void Vec3::SetX(float x)
    {                   
        m_x = x;
    }                   
                        
    INLINE void Vec3::SetY(float y)
    {                   
        m_y = y;
    }                   
                        
    INLINE void Vec3::SetZ(float z)
    {
        m_z = z;
    }

	INLINE float Vec3::operator[](uint index) const
	{
		return ( &m_x )[ index ];
	}

	INLINE float &Vec3::operator[](uint index)
	{
		return ( &m_x )[ index ];
	}

	INLINE const float *Vec3::GetFloatPtr(void) const
	{
		return &m_x;
	}

	// Operators
	INLINE bool Vec3::operator==(const Vec3 &rhs) const
	{
		return math::IsEqual( m_x, rhs.X( ) ) &&
			math::IsEqual( m_y, rhs.Y( ) ) &&
			math::IsEqual( m_z, rhs.Z( ) );
	}

	INLINE bool Vec3::operator!=(const Vec3 &rhs) const
	{
		return !( *this == rhs );
	}

	INLINE Vec3 Vec3::operator+(const Vec3 &rhs) const
	{
		return {
			m_x + rhs.X( ),
			m_y + rhs.Y( ),
			m_z + rhs.Z( )
		};
	}

	INLINE Vec3 Vec3::operator-(void) const
	{
		return {
			-m_x, -m_y, -m_z
		};
	}

	INLINE Vec3 Vec3::operator-(const Vec3 &rhs) const
	{
		return {
			m_x - rhs.X( ),
			m_y - rhs.Y( ),
			m_z - rhs.Z( )
		};
	}

	INLINE Vec3 Vec3::operator*(const Vec3 &rhs) const
	{
		return {
			m_x * rhs.X( ),
			m_y * rhs.Y( ),
			m_z * rhs.Z( )
		};
	}

	INLINE Vec3 Vec3::operator*(float rhs) const
	{
		return {
			m_x * rhs,
			m_y * rhs,
			m_z * rhs
		};
	}

	INLINE Vec3 operator*(float lhs, const Vec3 &rhs)
	{
		return {
			rhs.X( ) * lhs,
			rhs.Y( ) * lhs,
			rhs.Z( ) * lhs
		};
	}

	INLINE Vec3 Vec3::operator/(const Vec3 &rhs) const
	{
		return {
			m_x / rhs.X( ),
			m_y / rhs.Y( ),
			m_z / rhs.Z( )
		};
	}

	INLINE Vec3 Vec3::operator/(float rhs) const
	{
		float inv = 1.0f / rhs;

		return {
			m_x * inv,
			m_y * inv,
			m_z * inv
		};
	}

	INLINE const Vec3 &Vec3::operator=(const Vec3 &rhs)
	{
		m_x = rhs.X( );
		m_y = rhs.Y( );
		m_z = rhs.Z( );

		return *this;
	}

	INLINE const Vec3 &Vec3::operator+=(const Vec3 &rhs)
	{
		m_x += rhs.X( );
		m_y += rhs.Y( );
		m_z += rhs.Z( );

		return *this;
	}

	INLINE const Vec3 &Vec3::operator-=(const Vec3 &rhs)
	{
		m_x -= rhs.X( );
		m_y -= rhs.Y( );
		m_z -= rhs.Z( );

		return *this;
	}

	INLINE const Vec3 &Vec3::operator*=(const Vec3 &rhs)
	{
		m_x *= rhs.X( );
		m_y *= rhs.Y( );
		m_z *= rhs.Z( );

		return *this;
	}

	INLINE const Vec3 &Vec3::operator*=(float rhs)
	{
		m_x *= rhs;
		m_y *= rhs;
		m_z *= rhs;

		return *this;
	}

	INLINE const Vec3 &Vec3::operator/=(const Vec3 &rhs)
	{
		m_x /= rhs.X( );
		m_y /= rhs.Y( );
		m_z /= rhs.Z( );

		return *this;
	}

	INLINE const Vec3 &Vec3::operator/=(float rhs)
	{
		float inv = 1.0f / rhs;

		m_x *= inv;
		m_y *= inv;
		m_z *= inv;

		return *this;
	}
}