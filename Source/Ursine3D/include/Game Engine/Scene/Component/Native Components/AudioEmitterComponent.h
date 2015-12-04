#pragma once

#include "Component.h"
#include "AudioComponentBase.h"
#include "ListenerMasks.h"
#include <queue>

namespace ursine
{
	namespace ecs
	{
		class AudioEmitterComponent : public Component, public AudioComponentBase
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

			AudioEmitterComponent( void );
			~AudioEmitterComponent( void );

			float GetVolume( void ) const;
			void SetVolume( float volume );

			bool GetLoop( void ) const;
			void SetLoop( bool loop );

			bool GetMute( void ) const;
			void SetMute( bool mute );

			std::string GetFrontSound( void );
			void PopFrontSound( void );
			bool SoundsEmpty();
			void AddSoundToPlayQueue( std::string sound );

			Meta( Disable )
				void OnInitialize( void ) override;

		private:
			bool m_loop;
			bool m_mute;
			AkUInt32 m_listeners;
			float m_volume;
			
			// fire and forget
			std::queue<std::string> m_soundsFAF;

		} Meta( Enable, DisplayName( "Audio Emitter" ) );
	}
}