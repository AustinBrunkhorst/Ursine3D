/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Vec3.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#if defined(URSINE_DEPENDENCY_BulletPhysics)

#include "LinearMath/btVector3.h"

#endif

namespace ursine
{
	Vec3::Vec3(const SVec3 &value)
		: m_x( value.X( ) )
		, m_y( value.Y( ) )
		, m_z( value.Z( ) )
	{
	}

	Vec3::Vec3(const Vec2 &value, float Z)
		: m_x( value.X( ) )
		, m_y( value.Y( ) )
		, m_z( Z )
	{
	}

	Vec3::Vec3(const Vec4 &value)
		: m_x( value.X( ) )
		, m_y( value.Y( ) )
		, m_z( value.Z( ) )
	{
	}

	Vec3::Vec3(const SVec4 &value)
		: m_x( value.X( ) )
		, m_y( value.Y( ) )
		, m_z( value.Z( ) )
	{
	}

#if defined(URSINE_DEPENDENCY_BulletPhysics)
	Vec3::Vec3(const btVector3 &vec)
		: m_x( vec.getX( ) )
		, m_y( vec.getY( ) )
		, m_z( vec.getZ( ) )
	{
	}

	btVector3 Vec3::ToBullet(void) const
	{
		return btVector3( m_x, m_y, m_z );
	}
#endif

	// Public Methods
	void Vec3::Clean(void)
	{
		m_x = math::IsZero( m_x ) ? 0.0f : m_x;
		m_y = math::IsZero( m_y ) ? 0.0f : m_y;
		m_z = math::IsZero( m_z ) ? 0.0f : m_z;
	}

	std::string Vec3::ToString(void) const
	{
		std::ostringstream x_cvt;
		x_cvt << m_x;

		std::ostringstream y_cvt;
		y_cvt << m_y;

		std::ostringstream z_cvt;
		z_cvt << m_z;

		return { "{" +
			x_cvt.str( ) + ", " +
			y_cvt.str( ) + ", " +
			z_cvt.str( ) +
			"}" };
	}
}
