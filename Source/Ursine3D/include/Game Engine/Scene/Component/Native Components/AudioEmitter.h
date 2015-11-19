#pragma once

#include "Component.h"

#include "AudioID.h"
#include "AudioObject.h"
#include <Core/Audio/AudioManager.h>

namespace ursine
{
	namespace ecs
	{
		class AudioEmitter : public Component
		{
			NATIVE_COMPONENT;

		public:
			EditorField(
				AkUInt8 Volume,
				GetVolume,
				SetVolume
				);

			EditorField(
				bool Loop,
				GetLoop,
				SetLoop
				);

			AudioEmitter(void);
			~AudioEmitter(void);

			AkUInt8 GetVolume( );
			void SetVolume(AkUInt8 volume);

			bool GetLoop( );
			void SetLoop(bool loop);

			Meta(Disable)
				void OnInitialize(void) override;

		private:
			AkGameObjectID m_audioObjID;
			AkUInt8 m_volume;
			bool m_loop;

			void AudioEmitter::onTransformChange(EVENT_HANDLER(Entity));

		} Meta(Enable, DisplayName("Audio Emitter 3D"));
	}
}