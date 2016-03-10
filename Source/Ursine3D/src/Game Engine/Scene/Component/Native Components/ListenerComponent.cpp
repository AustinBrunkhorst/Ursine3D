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
			, m_listenerIndex( ListenerIndex( 0 ) ) { }

		AudioListener::~AudioListener(void)
		{

		}

		void AudioListener::OnInitialize(void)
		{

		}

		ListenerIndex AudioListener::GetListenerIndex(void) const
		{
			return m_listenerIndex;
		}

		void AudioListener::SetListenerIndex(ListenerIndex index)
		{
		#if defined(URSINE_WITH_EDITOR)

			auto audioSys = GetOwner( )->GetWorld( )->GetEntitySystem<AudioSystem>( );

			bool success = audioSys->ChangeAssignedListener( m_listenerIndex, index );

			// TODO: determine if this index is already being used 
			if (!success)
			{
				NotificationConfig config;

				config.type = NOTIFY_ERROR;
				config.dismissible = true;
				config.header = "Audio Listener";
				config.message = "This guy is already used brother.";

				EditorPostNotification( config );
				return;
			}

		#endif

			m_listenerIndex = index;

			NOTIFY_COMPONENT_CHANGED( "listener", m_listenerIndex );
		}
	}
}