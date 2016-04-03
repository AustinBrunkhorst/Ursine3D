/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEmitter.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "AudioComponentBase.h"
#include "ListenerMasks.h"
#include <queue>

namespace ursine
{
    namespace ecs
    {
        class AudioEmitter 
            : public Component
            , public AudioComponentBase
        {
            NATIVE_COMPONENT;

        public:
            EditorMeta(InputRange( 0.0f, 100.0f, 1, "{{value}}%" ))
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

            Meta(Enable)
            AudioEmitter(void);

            float GetVolume(void) const;
            void SetVolume(float volume);

            bool GetLoop(void) const;
            void SetLoop(bool loop);

            bool GetMute(void) const;
            void SetMute(bool mute);

            std::string GetFrontSound(void);
            void PopFrontSound(void);
            bool SoundsEmpty(void);
            void AddSoundToPlayQueue(const std::string &sound);

            bool PlayEvent(const resources::ResourceReference &event);

            ListenerIndex GetListeners(void);

        private:
            bool m_loop;
            bool m_mute;
            ListenerIndex m_listeners;
            float m_volume;
            
            // fire and forget
            std::queue<std::string> m_soundsFAF;
        } Meta(Enable, WhiteListMethods, DisplayName( "Audio Emitter" ));
    }
}