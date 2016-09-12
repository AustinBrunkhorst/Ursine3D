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

#include <AudioItemEventData.h>

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
            ~AudioEmitter(void);

            EditorResourceField(
                ursine::resources::AudioItemEventData,
                TestEvent,
                GetTestEvent,
                SetTestEvent
            );

            EditorButton(
                PushTestSound,
                "Play Sound"
            );

            EditorField(
                float attenuationScalingFactor,
                GetAttenuationScalingFactor,
                SetAttenuationScalingFactor
            );

            ListenerMask GetListenerMask(void) const;
            void SetListenerMask(ListenerMask mask);

            const ursine::resources::ResourceReference &GetTestEvent(void) const;
            void SetTestEvent(const ursine::resources::ResourceReference &audioEvent);

            float GetAttenuationScalingFactor(void) const;
            void SetAttenuationScalingFactor(float scalar);

            void PushEvent(const AudioEvent::Handle event);
            void PushEvent(const resources::ResourceReference &eventResource);

            AudioEvent::Handle GetEvent(void);
            void PopEvent(void);
            bool EmptyEvent(void);

            bool& checkStopFlag(void);

            void StopAllCurrentSounds(void);

        private:
            ListenerMask m_listenerMask;
            std::queue<AudioEvent::Handle> m_events;
            ursine::resources::ResourceReference m_testEvent;
            bool m_stopSounds;

            float m_attenuationScalar;

            void OnInitialize(void) override;
            
        } Meta(Enable, WhiteListMethods, DisplayName( "AudioEmitter" ));
    }
}