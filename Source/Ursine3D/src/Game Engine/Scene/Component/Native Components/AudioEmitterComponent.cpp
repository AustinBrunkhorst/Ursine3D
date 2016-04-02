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

#include "AudioItemEventData.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(AudioEmitter);

		AudioEmitter::AudioEmitter(void)
			: BaseComponent( )
			, m_maskChanged( false )
		    , m_listenerMask( ListenerMask::None )  { }

		ListenerMask AudioEmitter::GetListenerMask(void) const
		{
			return m_listenerMask;
		}

		void AudioEmitter::PushTestSound(void)
		{
			auto event = std::make_shared<AudioGeneric>( );

			event->name = m_testText;
			
			m_events.push( event );
		}

		const std::string& AudioEmitter::GetText(void) const
		{
			return m_testText;
		}

		void AudioEmitter::SetText(const std::string& text)
		{
			m_testText = text;
		}

		void AudioEmitter::PushEvent(const AudioEvent::Handle event)
		{
			m_events.push( event );
		}

		AudioEvent::Handle AudioEmitter::GetEvent(void)
		{
			return m_events.front( );
		}

		void AudioEmitter::PopEvent(void)
		{
			m_events.pop( );
		}

		bool AudioEmitter::EmptyEvent()
		{
			return m_events.empty( );
		}

		bool AudioEmitter::checkMask()
		{
			return m_maskChanged;
		}

		void AudioEmitter::ResetMaskFlag()
		{
			m_maskChanged = false;
		}

		void AudioEmitter::SetListenerMask(ListenerMask mask)
		{
			m_listenerMask = mask;

			m_maskChanged = true;

			NOTIFY_COMPONENT_CHANGED( "listenerMask", m_listenerMask );
		}
	}
}