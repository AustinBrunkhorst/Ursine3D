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
#include "AudioEventInfo.h"

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
            EditorMeta(BitMaskEditor)
            EditorField(
                ListenerMask listenerMask,
                GetListenerMask,
                SetListenerMask
            );

            Meta(Enable)
            AudioEmitter(void);

            EditorField(
                std::string test,
                GetText,
                SetText
            );

            EditorButton(
                PushTestSound,
                "Play Sound"
            );

            ListenerMask GetListenerMask(void) const;
            void SetListenerMask(ListenerMask mask);

            const std::string &GetText(void) const;
            void SetText(const std::string &text);

            void PushEvent(const AudioEvent::Handle event);
            void PushEvent(const resources::ResourceReference &eventResource);

            AudioEvent::Handle GetEvent(void);
            void PopEvent(void);
            bool EmptyEvent(void);

            bool checkMask(void);
            void ResetMaskFlag(void);

        private:
            ListenerMask m_listenerMask;
            std::queue<AudioEvent::Handle> m_events;
            std::string m_testText;
            bool m_maskChanged;
            
        } Meta(Enable, WhiteListMethods, DisplayName( "AudioEmitter" ));
    }
}