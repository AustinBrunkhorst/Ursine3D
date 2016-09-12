/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayAudioEventOnEntityAnimationFinishComponent.h
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <AudioEmitterComponent.h>

#include "EntityAnimatorComponent.h"

class PlayAudioEventOnEntityAnimationFinish : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        audioEvent,
        GetAudioEvent,
        SetAudioEvent
    );

    PlayAudioEventOnEntityAnimationFinish(void);
    ~PlayAudioEventOnEntityAnimationFinish(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const ursine::resources::ResourceReference &GetAudioEvent(void) const;
    void SetAudioEvent(const ursine::resources::ResourceReference &sfx);

private:
    ursine::resources::ResourceReference m_event;

    void onAnimationFinish(EVENT_HANDLER(EntityAnimator));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( ursine::ecs::AudioEmitter ),
        typeof( EntityAnimator )
    )
);
