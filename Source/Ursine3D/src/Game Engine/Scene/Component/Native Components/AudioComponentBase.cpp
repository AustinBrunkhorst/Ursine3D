/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioComponentBase.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AudioComponentBase.h"
#include "EntityEvent.h"
#include "AudioSystem.h"

namespace ursine
{
	AudioComponentBase::AudioComponentBase(void)
		: m_handle(AUDIO_UNALLOCATED_ID) 
	    , m_dirty(false) { }

	AkGameObjectID AudioComponentBase::GetHandle()
	{
		return m_handle;
	}

	void AudioComponentBase::OnInitialize(const ecs::EntityHandle &owner)
	{
		owner->Listener(this)
			.On(ecs::ENTITY_TRANSFORM_DIRTY, &AudioComponentBase::onTransformChange);
	}

	void AudioComponentBase::OnRemove(const ecs::EntityHandle &owner)
	{
		owner->Listener(this)
			.Off(ecs::ENTITY_TRANSFORM_DIRTY, &AudioComponentBase::onTransformChange);
	}

	void AudioComponentBase::onTransformChange(EVENT_HANDLER(ecs::Entity))
	{
		m_dirty = true;
	}
}