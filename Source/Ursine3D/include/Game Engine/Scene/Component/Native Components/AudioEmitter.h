#pragma once

#include "Component.h"
#include "AudioComponentBase.h"
#include "ListenerMasks.h"
#include <queue>

namespace ursine
{
	namespace ecs
	{
		class AudioEmitter : public Component, public AudioComponentBase
		{
			NATIVE_COMPONENT;

		public:

			EditorField(
				float Volume,
				GetVolume,
				SetVolume
				);

			EditorField(
				bool Loop,
				GetLoop,
				SetLoop
				);

			EditorField(
				bool Mute,
				GetMute,
				SetMute
				);

			AudioEmitter(void);
			~AudioEmitter(void);

			float GetVolume( ) const;
			void SetVolume(float volume);

			bool GetLoop( ) const;
			void SetLoop(bool loop);

			bool GetMute() const;
			void SetMute(bool mute);

			Meta(Disable)
				void OnInitialize(void) override;

		private:
			bool m_loop;
			bool m_mute;
			AkUInt32 m_listeners;
			float m_volume;
			
			// fire and forget
			std::queue<std::string> m_soundsFAF;

		} Meta(Enable, DisplayName("Audio Emitter"));
	}
}