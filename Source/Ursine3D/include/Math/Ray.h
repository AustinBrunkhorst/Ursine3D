/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Ray.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "SVec3.h"

namespace ursine
{
	class Ray
	{
	public:
		Ray(void);
		Ray(const SVec3 &origin, const SVec3 &dir);

		const SVec3 &GetOrigin(void) const;
		void SetOrigin(const SVec3 &origin);

		const SVec3 &GetDirection(void) const;
		void SetDirection(const SVec3 &dir);

		SVec3 GetPoint(float t) const;

	private:
		// The origin of the ray
		SVec3 m_origin;

		// The normalized direction of this ray
		SVec3 m_dir;
	};
}

#include "Ray.hpp"
