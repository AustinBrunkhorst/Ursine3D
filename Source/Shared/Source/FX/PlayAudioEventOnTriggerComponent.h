/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayAudioEventOnTriggerComponent.h
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <AudioEmitterComponent.h>
#include <GhostComponent.h>

class PlayAudioEventOnTrigger : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        audioEvent,
        GetAudioEvent,
        SetAudioEvent
    );

    ursine::Array<std::string> triggerEntities;

    PlayAudioEventOnTrigger(void);
    ~PlayAudioEventOnTrigger(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const ursine::resources::ResourceReference &GetAudioEvent(void) const;
    void SetAudioEvent(const ursine::resources::ResourceReference &sfx);

private:
    ursine::resources::ResourceReference m_event;

    void onTrigger(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( ursine::ecs::AudioEmitter ),
        typeof( ursine::ecs::Ghost )
    )
);
