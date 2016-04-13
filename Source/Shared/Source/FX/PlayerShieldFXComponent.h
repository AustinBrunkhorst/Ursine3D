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
#include <AudioItemEventData.h>

#include "HealthComponent.h"

class PlayerShieldFX : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorResourceField(
        ursine::resources::AudioItemEventData,
        shieldBreakSfx,
        GetShieldBreakSfx,
        SetShieldBreakSfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        shieldRebuildSfx,
        GetShieldRebuildSfx,
        SetShieldRebuildSfx
    );

    PlayerShieldFX(void);
    ~PlayerShieldFX(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const ursine::resources::ResourceReference &GetShieldBreakSfx(void) const;
    void SetShieldBreakSfx(const ursine::resources::ResourceReference &shieldBreakSfx);

    const ursine::resources::ResourceReference &GetShieldRebuildSfx(void) const;
    void SetShieldRebuildSfx(const ursine::resources::ResourceReference &shieldRebuildSfx);

private:
    bool m_destroyed;

    ursine::resources::ResourceReference m_shieldBreakSfx;
    ursine::resources::ResourceReference m_shieldRebuildSfx;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));
} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( Health )
    )
);
