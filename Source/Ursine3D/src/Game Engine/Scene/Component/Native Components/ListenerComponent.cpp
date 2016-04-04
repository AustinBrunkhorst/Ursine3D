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
			, m_mask( ListenerMask::L1 )
			, m_listenerIndex( ListenerIndex::L1 ) { }

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

			indexToMask( );

			m_dirty = true;

			NOTIFY_COMPONENT_CHANGED( "listener", m_listenerIndex );
		}

		ListenerMask AudioListener::GetListenerMask(void)
		{
			return m_mask;
		}

		void AudioListener::indexToMask(void)
		{
			switch (m_listenerIndex)
			{
			case ListenerIndex::L1:
				m_mask = ListenerMask::L1;
				break;
			case ListenerIndex::L2:
				m_mask = ListenerMask::L2;
				break;
			case ListenerIndex::L3:
				m_mask = ListenerMask::L3;
				break;
			case ListenerIndex::L4:
				m_mask = ListenerMask::L4;
				break;
			case ListenerIndex::L5:
				m_mask = ListenerMask::L5;
				break;
			case ListenerIndex::L6:
				m_mask = ListenerMask::L6;
				break;
			case ListenerIndex::L7:
				m_mask = ListenerMask::L7;
				break;
			case ListenerIndex::LG:
				m_mask = ListenerMask::L8;
				break;
			}
		}
	}
}