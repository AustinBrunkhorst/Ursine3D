#include "UrsinePrecompiled.h"

#include "AudioComponentBase.h"
#include "EntityEvent.h"

namespace ursine
{
	AudioComponentBase::AudioComponentBase(void)
		: m_handle(AUDIO_UNALLOCATED_ID) 
	    , m_dirty(false) { }

	void AudioComponentBase::OnInitialize(ecs::Entity* owner)
	{
		owner->Listener(this)
			.On(ecs::ENTITY_TRANSFORM_DIRTY, &AudioComponentBase::onTransformChange);
	}

	void AudioComponentBase::OnRemove(ecs::Entity *owner)
	{
		owner->Listener(this)
			.Off(ecs::ENTITY_TRANSFORM_DIRTY, &AudioComponentBase::onTransformChange);
	}

	void AudioComponentBase::onTransformChange(EVENT_HANDLER(ecs::Entity))
	{
		m_dirty = true;
	}
}