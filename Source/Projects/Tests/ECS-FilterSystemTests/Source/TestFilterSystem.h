/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TestFilterSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

namespace ursine
{
	namespace ecs
	{
		class TestFilterSystem : public FilterSystem
		{
			ENTITY_SYSTEM;

		public:
			static uint numUpdated;

			TestFilterSystem(World *world);

		protected:
			void Process(Entity *entity) override;
			
		} Meta(Enable);
	}
}
