/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ResetParticleCountOnAnimationEventComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <AnimatorComponent.h>
#include <ParticleEmitterComponent.h>

class ResetParticleCountOnAnimationEvent : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    std::string animStateName;
    std::string animEventMessage;

    ResetParticleCountOnAnimationEvent(void);

    void OnSceneReady(ursine::Scene *scene) override;

private:

    void onAnimEvent(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( ursine::ecs::ParticleEmitter )
    )
);
