/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"

namespace ursine
{
	namespace ecs
	{
		class WorldConfig : public Component
		{
			NATIVE_COMPONENT;

		public:
            Meta(Enable)
            WorldConfig(void);
			~WorldConfig(void);

		private:
            void OnInitialize(void) override;

		} Meta(Enable, WhiteListMethods, DisableComponentRemoval);
	}
}