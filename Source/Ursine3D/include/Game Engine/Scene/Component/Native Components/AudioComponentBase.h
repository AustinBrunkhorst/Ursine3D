/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioComponentBase.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "AudioID.h"
#include "EntityConfig.h"
#include "EventDispatcher.h"

namespace ursine
{
	namespace ecs
	{
		class AudioSystem;
	}

	class AudioComponentBase
	{
	public:
		friend class ecs::AudioSystem;

		AudioComponentBase(void);
		AkGameObjectID GetHandle();

		void OnInitialize(ecs::Entity *owner);
		void OnRemove(ecs::Entity *owner);

	protected:

		AkGameObjectID m_handle;

		// lets us know if we need to update the matrix in the renderer
		bool m_dirty;

		void onTransformChange(EVENT_HANDLER(ecs::Entity));
	};
}