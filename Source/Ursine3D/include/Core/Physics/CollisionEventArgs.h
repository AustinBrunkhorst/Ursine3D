/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CollisionEventArgs.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EventArgs.h"

namespace ursine
{
	namespace ecs
	{
		class Entity;
	}

	namespace physics
	{
		class Contact
		{
		public:
			// Normal (from other to this) and point (in world coordinates) of contact
			SVec3 normal, point;

			// The relative velocity between the body and this contact (treating it as
			// if it were attached to the other object)
			SVec3 relativeVelocity;

			// The most recent applied impulse, to satisfy contact constraints by the constraint solver
			float appliedImpulse;

			float penDistance;

			Contact(void);
		};

		class CollisionEventArgs : public EventArgs
		{
		public:
			std::vector<Contact> contacts;
			
			ecs::Entity *thisEntity, *otherEntity;
		};
	}
}
