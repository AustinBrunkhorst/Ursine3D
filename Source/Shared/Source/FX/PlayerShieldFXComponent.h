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
#include <Color.h>

#include "HealthComponent.h"
#include "FragmentationComponent.h"

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

    EditorField(
        ursine::Color flashColor,
        GetFlashColor,
        SetFlashColor
    );

    EditorField(
        ursine::Color flashColor,
        GetFlashColor,
        SetFlashColor
    );

    EditorField(
        float flashDuration,
        GetFlashDuration,
        SetFlashDuration
    );

    PlayerShieldFX(void);
    ~PlayerShieldFX(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const ursine::resources::ResourceReference &GetShieldBreakSfx(void) const;
    void SetShieldBreakSfx(const ursine::resources::ResourceReference &shieldBreakSfx);

    const ursine::resources::ResourceReference &GetShieldRebuildSfx(void) const;
    void SetShieldRebuildSfx(const ursine::resources::ResourceReference &shieldRebuildSfx);

    const ursine::Color &GetFlashColor(void) const;
    void SetFlashColor(const ursine::Color &color);

    float GetFlashDuration(void) const;
    void SetFlashDuration(float duration);

private:
    bool m_destroyed;

    ursine::resources::ResourceReference m_shieldBreakSfx;
    ursine::resources::ResourceReference m_shieldRebuildSfx;

    ursine::Color m_flashColor;

    float m_duration;
    float m_timer;

    float m_initTimer;

    std::vector<ursine::ecs::ModelFragmenter*> m_fragmenters;
    std::vector<ursine::Color> m_defaultColors;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));
    void onHealthDamaged(EVENT_HANDLER(Health));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( Health )
    )
);
