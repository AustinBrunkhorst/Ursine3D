#pragma once

#include "Component.h"

#include "WwiseForward.h"
#include "SVec3.h"
#include <Core/Audio/AudioManager.h>

namespace ursine
{
	namespace ecs
	{
		class Emitter : public Component
		{
			NATIVE_COMPONENT;

		public:
			EditorField(
				AkGameObjectID id,
				GetID,
				SetID
				);

			EditorField(
				SVec3 Direction,
				GetDirection,
				SetDirection
				);

			EditorField(
				SVec3 Direction,
				GetDirection,
				SetDirection
				);

			Emitter(void);
			~Emitter(void);

			Meta(Disable)
				void OnInitialize(void) override;

			const AkGameObjectID GetID(void);
			void SetID(const AkGameObjectID id);

			const SVec3 &GetPosition(void);
			void SetDirection(const SVec3 &pos);

			const SVec3 &GetOrientation(void);
			void SetOrientation(const SVec3 &orien);


		private:
			AudioManager *m_audio;
			AkGameObjectID m_object_id;
			SVec3 m_object_pos;
			SVec3 m_object_orientation;
			bool m_dirty;

			friend class RenderSystem;
		} Meta(Enable, DisplayName("Emitter"));
	}
}