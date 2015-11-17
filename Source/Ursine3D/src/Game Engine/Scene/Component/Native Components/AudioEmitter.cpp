#include "UrsinePrecompiled.h"

#include "AudioEmitter.h"
#include "EntityEvent.h"
#include "AudioSystem.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(AudioEmitter);

		AudioEmitter::AudioEmitter(void)
			: BaseComponent()
			, m_audioObjID(UNALLOCATED_ID)
		    , m_loop(false)  { }

		AkUInt8 AudioEmitter::GetVolume()
		{
			return m_volume;
		}

		void AudioEmitter::SetVolume(AkUInt8 volume)
		{
			m_volume = volume;
		}

		bool AudioEmitter::GetLoop()
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