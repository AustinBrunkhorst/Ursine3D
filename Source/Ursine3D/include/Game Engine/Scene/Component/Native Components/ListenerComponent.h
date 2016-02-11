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
		class AudioListener 
            : public Component
            , public AudioComponentBase
		{
			NATIVE_COMPONENT;

		public:
            Meta(Enable)
			AudioListener(void);
			~AudioListener(void);

            ursine::Array<ursine::Color> testing;

			ListenerIndex GetListenerIndex(void);
			void SetListenerIndex(ListenerIndex index);

			void OnInitialize(void) override;

		private:
			 ListenerIndex m_listenerIndex;

		} Meta(Enable, WhiteListMethods, DisplayName( "Audio Listener 3D" ));
	}
}