/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Plane.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "SVec3.h"
#include "Ray.h"

namespace ursine
{
	class Plane
	{
	public:
		Plane(void);
		Plane(const SVec3 &normal, float d);

		// Construct a plane from 3 points on it
		Plane(const SVec3 &point1, const SVec3 &point2, const SVec3 &point3);

		// Construct a plane from a point on it and a normal
		Plane(const SVec3 &point, const SVec3 &normal);

		SVec3 ClosestPoint(const Ray &ray) const;

		// Project the given point onto the plane
		SVec3 Project(const SVec3 &point) const;

		const SVec3 &GetNormal(void) const;

		const float &GetD(void) const;

	private:
		// The direction the plane is faceing. The direction vector is always normalized.
		SVec3 m_normal;

		// The offset of this plane from the origin. The value -d gives the signed distance
		// of this plane from the origin.
		float m_d;
	};
}

#include "Plane.hpp"
