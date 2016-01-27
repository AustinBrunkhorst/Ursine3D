/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Ray.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** --------------------------------------------------------------------------*/

namespace ursine
{
	INLINE Ray::Ray(void)
	{
	}

	INLINE Ray::Ray(const SVec3& origin, const SVec3& dir)
	{
		m_origin = origin;
		m_dir = dir;

		m_dir.Normalize( );
	}

	INLINE const SVec3& Ray::GetOrigin(void) const
	{
		return m_origin;
	}

	INLINE void Ray::SetOrigin(const SVec3& origin)
	{
		m_origin = origin;
	}

	INLINE const SVec3& Ray::GetDirection(void) const
	{
		return m_dir;
	}

	INLINE void Ray::SetDirection(const SVec3& dir)
	{
		m_dir = SVec3::Normalize( dir );
	}

	INLINE SVec3 Ray::GetPoint(float t) const
	{
		return m_origin + m_dir * t;
	}
}
