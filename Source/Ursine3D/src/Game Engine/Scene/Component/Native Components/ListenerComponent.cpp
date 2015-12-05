#include "UrsinePrecompiled.h"

#include "ListenerComponent.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(AudioListener);

		AudioListener::AudioListener()
			: BaseComponent()
			, m_listenerIndex( ListenerIndex::None )
		{ }

		void AudioListener::SetListenerIndex(ListenerIndex index)
		{
			m_listenerIndex = index;
		}

		ListenerIndex AudioListener::GetListenerIndex()
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