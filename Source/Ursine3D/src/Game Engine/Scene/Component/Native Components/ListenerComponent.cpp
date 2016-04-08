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

#if defined(URSINE_WITH_EDITOR)

#include "Notification.h"

#endif

#include "SystemManager.h"
#include "AudioSystem.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION( AudioListener );

		AudioListener::AudioListener(void)
			: BaseComponent( )
            , m_listenerIndex( ListenerIndex::NONE ) { }

		AudioListener::~AudioListener(void)
		{
            AudioComponentBase::OnRemove( GetOwner( ) );
		}

		void AudioListener::OnInitialize(void)
		{
            AudioComponentBase::OnInitialize( GetOwner( ) );
		}

		ListenerIndex AudioListener::GetListenerIndex(void) const
		{
			return m_listenerIndex;
		}

		void AudioListener::SetListenerIndex(ListenerIndex index)
		{
			m_listenerIndex = index;

			NOTIFY_COMPONENT_CHANGED( "listener", m_listenerIndex );
		}

	}
}