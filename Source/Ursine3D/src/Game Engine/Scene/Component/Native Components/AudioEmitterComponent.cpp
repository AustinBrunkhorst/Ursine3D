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
		NATIVE_COMPONENT_DEFINITION(AudioEmitterComponent);

		AudioEmitterComponent::AudioEmitterComponent(void)
			: BaseComponent()
		    , m_loop(false)
			, m_mute(false)
		    , m_listeners(ListenerIndex::One)
		    , m_volume(100.0f)  { }

		float AudioEmitterComponent::GetVolume() const
		{
			return m_volume;
		}

		void AudioEmitterComponent::SetVolume(float volume)
		{
			m_volume = math::Clamp(volume, 0.0f, 100.0f);
			NOTIFY_COMPONENT_CHANGED("Volume", m_volume);
		}

		bool AudioEmitterComponent::GetMute() const
		{
			return m_mute;
		}

		void AudioEmitterComponent::SetMute(bool mute)
		{
			m_mute = mute;
		}

		bool AudioEmitterComponent::GetLoop() const
		{
			return m_loop;
		}

		void AudioEmitterComponent::SetLoop(bool loop)
		{
			m_loop = loop;
		}

		AudioEmitterComponent::~AudioEmitterComponent(void)
		{
		
		}

		void AudioEmitterComponent::OnInitialize(void)
		{
		}

		std::string AudioEmitterComponent::GetFrontSound(void)
		{
			return m_soundsFAF.front();
		}

		void AudioEmitterComponent::PopFrontSound(void)
		{
			m_soundsFAF.pop();
		}

		bool AudioEmitterComponent::SoundsEmpty()
		{
			return m_soundsFAF.empty();
		}

		void AudioEmitterComponent::AddSoundToPlayQueue(std::string sound)
		{
			if (!m_mute)
				m_soundsFAF.push( sound );
		}

		ListenerIndex AudioEmitterComponent::GetListeners()
		{
			return m_listeners;
		}
	}
}