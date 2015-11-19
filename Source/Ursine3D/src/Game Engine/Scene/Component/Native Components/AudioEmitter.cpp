#include "UrsinePrecompiled.h"

#include "AudioEmitter.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(AudioEmitter);

		AudioEmitter::AudioEmitter(void)
			: BaseComponent()
		    , m_loop(false)
			, m_mute(false)
		    , m_listeners(LISTENER_NONE)
		    , m_volume(100.0f)  { }

		float AudioEmitter::GetVolume() const
		{
			return m_volume;
		}

		void AudioEmitter::SetVolume(float volume)
		{
			m_volume = math::Clamp(volume, 0.0f, 100.0f);
			NOTIFY_COMPONENT_CHANGED("Volume", m_volume);
		}

		bool AudioEmitter::GetMute() const
		{
			return m_mute;
		}

		void AudioEmitter::SetMute(bool mute)
		{
			m_mute = mute;
		}

		bool AudioEmitter::GetLoop() const
		{
			return m_loop;
		}

		void AudioEmitter::SetLoop(bool loop)
		{
			m_loop = loop;
		}

		AudioEmitter::~AudioEmitter(void)
		{
		
		}

		void AudioEmitter::OnInitialize(void)
		{
		}
	}
}