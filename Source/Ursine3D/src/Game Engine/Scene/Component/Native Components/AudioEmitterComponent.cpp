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
		    , m_loop( false )
			, m_mute( false )
		    , m_listeners( ListenerIndex::One )
		    , m_volume( 100.0f )  { }

		float AudioEmitter::GetVolume(void) const
		{
			return m_volume;
		}

		void AudioEmitter::SetVolume(float volume)
		{
			m_volume = math::Clamp( volume, 0.0f, 100.0f );

			NOTIFY_COMPONENT_CHANGED( "Volume", m_volume );
		}

		bool AudioEmitter::GetMute(void) const
		{
			return m_mute;
		}

		void AudioEmitter::SetMute(bool mute)
		{
			m_mute = mute;
		}

		bool AudioEmitter::GetLoop(void) const
		{
			return m_loop;
		}

		void AudioEmitter::SetLoop(bool loop)
		{
			m_loop = loop;
		}

		std::string AudioEmitter::GetFrontSound(void)
		{
			return m_soundsFAF.front( );
		}

		void AudioEmitter::PopFrontSound(void)
		{
			m_soundsFAF.pop( );
		}

		bool AudioEmitter::SoundsEmpty(void)
		{
			return m_soundsFAF.empty( );
		}

		void AudioEmitter::AddSoundToPlayQueue(const std::string &sound)
		{
			if (!m_mute)
				m_soundsFAF.push( sound );
		}

		ListenerIndex AudioEmitter::GetListeners(void)
		{
			return m_listeners;
		}
	}
}