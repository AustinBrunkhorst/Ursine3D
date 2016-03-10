/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEmitterComponent.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AudioEmitterComponent.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(AudioEmitter);

		AudioEmitter::AudioEmitter(void)
			: BaseComponent( )
		    , m_listenerMask( ListenerMask::None )  { }

		ListenerMask AudioEmitter::GetListenerMask(void) const
		{
			return m_listenerMask;
		}

		void AudioEmitter::SetListenerMask(ListenerMask mask)
		{
			m_listenerMask = mask;

			NOTIFY_COMPONENT_CHANGED( "listenerMask", m_listenerMask );
		}
	}
}