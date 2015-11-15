#include "UrsinePrecompiled.h"

#include "EmitterComponent.h"
#include "ComponentConfig.h

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(Emitter);

		Emitter::Emitter(void)
			: BaseComponent()
			, m_object_id(AK_INVALID_GAME_OBJECT)
			, m_object_orientation(1, 0, 0)
		    , m_object_pos(0)
		    , m_dirty(false) { }

		Emitter::~Emitter(void)
		{
		}

		void Emitter::OnInitialize(void)
		{
			m_audio = GetCoreSystem(AudioManager);

			auto *owner = GetOwner();

			Emitter *emitter;

			URSINE_TODO("Decouple this");
			if (owner->HasComponent<Emitter>())
			{
				emitter = owner->GetComponent<Emitter>();
			}
			else
			{
				emitter = owner->AddComponent<Emitter>();
			}

			m_audio->SetObject3DPosition(m_object_id, m_object_pos, m_object_orientation);
		}

		const SVec3& Emitter::GetPosition()
		{
			return m_object_pos;
		}

		void Emitter::SetDirection(const SVec3& pos)
		{
			m_object_pos = pos;

			m_audio->SetObject3DPosition(m_object_id, m_object_pos, m_object_orientation);

			NOTIFY_COMPONENT_CHANGED("Object Position", pos);
		}

		const SVec3& Emitter::GetOrientation()
		{
			return m_object_orientation;
		}

		void Emitter::SetOrientation(const SVec3& orien)
		{
			m_object_orientation = orien;

			m_audio->SetObject3DPosition(m_object_id, m_object_pos, m_object_orientation);

			NOTIFY_COMPONENT_CHANGED("Object Orientation", orien);
		}
		
	}
}