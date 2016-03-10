/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEmitter.h
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
		class AudioEmitter 
            : public Component
            , public AudioComponentBase
		{
			NATIVE_COMPONENT;

		public:
			EditorMeta(BitMaskEditor)
            EditorField(
				ListenerMask listenerMask,
                GetListenerMask,
                SetListenerMask
            );

            Meta(Enable)
			AudioEmitter(void);

			ListenerMask GetListenerMask(void) const;
			void SetListenerMask(ListenerMask mask);

		private:
			ListenerMask m_listenerMask;

		} Meta(Enable, WhiteListMethods, DisplayName( "AudioEmitter" ));
	}
}