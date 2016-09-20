/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayAudioEventComponent.h
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <AudioEmitterComponent.h>

class PlayAudioEvent : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        audioEvent,
        GetAudioEvent,
        SetAudioEvent
    );

    EditorField(
        bool playOnAwake,
        GetPlayOnAwake,
        SetPlayOnAwake
    );

    PlayAudioEvent(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const ursine::resources::ResourceReference &GetAudioEvent(void) const;
    void SetAudioEvent(const ursine::resources::ResourceReference &sfx);

    bool GetPlayOnAwake(void) const;
    void SetPlayOnAwake(bool toggle);

    void Play(void);

private:
    ursine::resources::ResourceReference m_event;

    bool m_playOnAwake;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( ursine::ecs::AudioEmitter )
    )
);
