/* ---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SweptControllerSystem.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "FilterSystem.h"

namespace ursine
{
	namespace ecs
	{
		class SweptControllerSystem : public FilterSystem
		{
			ENTITY_SYSTEM;

		public:

			Meta(DisableNonDynamic)
			SweptControllerSystem(World *world);

		private:
			void Process(Entity *entity) override;

		} Meta(Enable);
	}
}
