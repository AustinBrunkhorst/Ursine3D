/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEmitterEnableDelayComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <ParticleEmitterComponent.h>

class ParticleEmitterEnableDelay : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        float delay,
        GetDelay,
        SetDelay
    );

    ParticleEmitterEnableDelay(void);
    ~ParticleEmitterEnableDelay(void);

    void OnSceneReady(ursine::Scene *scene) override;

    float GetDelay(void) const;
    void SetDelay(float delay);

private:

    float m_delay;

    float m_timer;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( ursine::ecs::ParticleEmitter )
    )
);
