/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Vec4.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#ifdef URSINE_GRAPHICS_DIRECTX
#include "DirectXMath.h"
#endif

namespace ursine
{
	SVec4::SVec4(const SVec3 &value, float W)
		: m_x( value.X( ) )
		  , m_y( value.Y( ) )
		  , m_z( value.Z( ) )
		  , m_w( W )
	{
	}

	SVec4::SVec4(const Vec3 &value, float W)
		: m_x( value.X( ) )
		  , m_y( value.Y( ) )
		  , m_z( value.Z( ) )
		  , m_w( W )
	{
	}

	SVec4::SVec4(const Vec4 &value)
		: m_x( value.X( ) )
		  , m_y( value.Y( ) )
		  , m_z( value.Z( ) )
		  , m_w( value.m_w )
	{
	}

#ifdef URSINE_GRAPHICS_DIRECTX
	SVec4::SVec4(const DirectX::XMFLOAT4 &vec)
		: m_x(vec.x)
		, m_y(vec.y)
		, m_z(vec.z)
		, m_w(vec.w)
	{
	}

	DirectX::XMFLOAT4 SVec4::ToDxVec(void) const
	{
		return { m_x, m_y, m_z, m_w };
	}
#endif

	// Public Methods
	void SVec4::Clean(void)
	{
		m_x = math::IsZero( m_x ) ? 0.0f : m_x;
		m_y = math::IsZero( m_y ) ? 0.0f : m_y;
		m_z = math::IsZero( m_z ) ? 0.0f : m_z;
		m_w = math::IsZero( m_w ) ? 0.0f : m_w;
	}

	std::string SVec4::ToString(void) const
	{
		std::ostringstream x_cvt;
		x_cvt << m_x;

		std::ostringstream y_cvt;
		y_cvt << m_y;

		std::ostringstream z_cvt;
		z_cvt << m_z;

		std::ostringstream w_cvt;
		w_cvt << m_w;

		return { "{" +
			x_cvt.str( ) + ", " +
			y_cvt.str( ) + ", " +
			z_cvt.str( ) + ", " +
			w_cvt.str( ) +
			"}" };
	}
}
