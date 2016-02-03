/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ListenerComponent.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ListenerComponent.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION( AudioListener );

		AudioListener::AudioListener()
			: BaseComponent( )
			, m_listenerIndex( ListenerIndex::None ) { }

		void AudioListener::SetListenerIndex(ListenerIndex index)
		{
			m_listenerIndex = index;
		}

		ListenerIndex AudioListener::GetListenerIndex(void)
		{
			return m_listenerIndex;
		}

		AudioListener::~AudioListener(void)
		{

		}

		void AudioListener::OnInitialize(void)
		{
		}
	}
}