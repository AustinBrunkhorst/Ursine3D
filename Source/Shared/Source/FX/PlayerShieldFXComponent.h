/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerShieldFXComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "HealthComponent.h"

class PlayerShieldFX : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    PlayerShieldFX(void);
    ~PlayerShieldFX(void);

    void OnSceneReady(ursine::Scene *scene) override;

private:
    bool m_destroyed;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));
} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( Health )
    )
);
