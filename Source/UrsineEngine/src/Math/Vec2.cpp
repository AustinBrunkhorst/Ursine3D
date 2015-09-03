/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Vec2.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace ursine
{
	Vec2::Vec2(const Vec3 &value)
		: m_x( value.m_x )
		, m_y( value.m_y )
	{
	}

	Vec2::Vec2(const SVec3 &value)
		: m_x( value.m_x )
		, m_y( value.m_y )
	{
	}

	// Public Methods
	void Vec2::Clean(void)
	{
		m_x = math::IsZero( m_x ) ? 0.0f : m_x;
		m_y = math::IsZero( m_y ) ? 0.0f : m_y;
	}

	std::string Vec2::ToString(void) const
	{
		std::ostringstream x_cvt;
		x_cvt << m_x;

		std::ostringstream y_cvt;
		y_cvt << m_y;

		return { "{" +
			x_cvt.str( ) + ", " +
			y_cvt.str( ) +
			"}" };
	}
}