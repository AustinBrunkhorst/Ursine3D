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
			EditorField(
				ListenerIndex listener,
				GetListenerIndex, 
				SetListenerIndex
			);

            Meta(Enable)
			AudioListener(void);
			~AudioListener(void);

			void OnInitialize(void) override;

			ListenerIndex GetListenerIndex(void) const;
			void SetListenerIndex(ListenerIndex index);

			ListenerMask GetListenerMask(void);

		private:
			 ListenerIndex m_listenerIndex;
			 ListenerMask m_mask;

			 void indexToMask(void);

		} Meta(Enable, WhiteListMethods, DisplayName( "AudioListener" ));
	}
}