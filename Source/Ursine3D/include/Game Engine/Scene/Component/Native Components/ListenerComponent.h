/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ListenerComponent.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "AudioComponentBase.h"
#include "ListenerMasks.h"

namespace ursine
{
	namespace ecs
	{
		class AudioListener : public Component, public AudioComponentBase
		{
			NATIVE_COMPONENT;

		public:

			AudioListener();
			~AudioListener(void);

			ListenerIndex GetListenerIndex();
			void SetListenerIndex(ListenerIndex index);

			Meta(Disable)
				void OnInitialize(void) override;

		private:
			 ListenerIndex m_listenerIndex;

		} Meta(Enable, DisplayName("Audio Listener 3D"));
	}
}