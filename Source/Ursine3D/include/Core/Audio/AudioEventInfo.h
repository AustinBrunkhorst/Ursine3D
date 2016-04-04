/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEventInfo.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "WWiseForward.h"

namespace ursine
{
	namespace ecs
	{
		struct AudioEvent
		{
			typedef std::shared_ptr<AudioEvent> Handle;

			virtual ~AudioEvent(void) { }

			virtual void ApplyParams(AkGameObjectID id) = 0;
		};

		struct AudioSwitch : AudioEvent
		{
			std::string switchGroup;
			std::string switchState;

			void ApplyParams(AkGameObjectID id) override;
		};

		struct AudioState : AudioEvent
		{
			std::string stateGroup;
			std::string audioState;

			void ApplyParams(AkGameObjectID id) override;
		};

		struct AudioRTPC : AudioEvent
		{
			std::string parameter;
			float val;

			void ApplyParams(AkGameObjectID id) override;
		};

		struct AudioTrigger : AudioEvent
		{
			std::string name;

			void ApplyParams(AkGameObjectID id) override;
		};

		struct AudioGeneric : AudioEvent
		{
			std::string name;

			void ApplyParams(AkGameObjectID id) override;
		};
	}
}