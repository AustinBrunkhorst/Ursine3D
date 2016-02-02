/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Plane.hpp
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** --------------------------------------------------------------------------*/

namespace ursine
{
	INLINE Plane::Plane(void)
		: m_d( 0 )
	{
	}

	INLINE Plane::Plane(const SVec3& normal, float d)
		: m_normal( normal )
		, m_d( d )
	{
	}

	INLINE Plane::Plane(const SVec3& point1, const SVec3& point2, const SVec3& point3)
	{
		m_normal = SVec3::Cross( point2 - point1, point3 - point1 );

		m_normal.Normalize( );

		m_d = m_normal.Dot( point1 );
	}

	INLINE Plane::Plane(const SVec3& point, const SVec3& normal)
	{
		m_normal = normal;

		m_d = point.Dot( m_normal );
	}

	INLINE SVec3 Plane::ClosestPoint(const Ray& ray) const
	{
		// The plane and a ray have three configurations:
		// 1) The ray and the plane don' intersect: the closest point is the ray's origin
		// 2) The ray and the plane do intersect: The closest point is the intersection point
		// 3) The ray is parallel to the plane: any point on the ray projected to the plane is a closest point.
		float denom = SVec3::Dot( m_normal, ray.GetDirection( ) );

		if (denom == 0.f)
			return Project( ray.GetOrigin( ) ); // case 3)

		float t = (m_d - SVec3::Dot( m_normal, ray.GetOrigin( ) )) / denom;

		// Numerical stability check: Instead of checking denom against epsilon, 
		// check the resulting t for very large values.
		if (t >= 0.0f && t < 1e6f)
			return ray.GetPoint( t ); // case 2)
		else
			return Project( ray.GetOrigin( ) ); // case 1)
	}

	INLINE SVec3 Plane::Project(const SVec3& point) const
	{
		return point - (m_normal.Dot( point ) - m_d) * m_normal;
	}

	INLINE const SVec3& Plane::GetNormal(void) const
	{
		return m_normal;
	}

	INLINE const float& Plane::GetD(void) const
	{
		return m_d;
	}
}
