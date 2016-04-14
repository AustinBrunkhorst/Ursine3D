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

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        hurtSfx,
        GetHurtSfx,
        SetHurtSfx
    );

    EditorField(
        float hurtSfxMinDelay,
        GetHurtSfxMinDelay,
        SetHurtSfxMinDelay
    );

    EditorField(
        float hurtSfxMaxDelay,
        GetHurtSfxMaxDelay,
        SetHurtSfxMaxDelay
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        heartBeatPlaySfx,
        GetHeartBeatPlaySfx,
        SetHeartBeatPlaySfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        heartBeatStopSfx,
        GetHeartBeatStopSfx,
        SetHeartBeatStopSfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        bossShieldDownSfx,
        GetBossShieldDownSfx,
        SetBossShieldDownSfx
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

    const ursine::resources::ResourceReference &GetHurtSfx(void) const;
    void SetHurtSfx(const ursine::resources::ResourceReference &sfx);

    float GetHurtSfxMinDelay(void) const;
    void SetHurtSfxMinDelay(float delay);

    float GetHurtSfxMaxDelay(void) const;
    void SetHurtSfxMaxDelay(float delay);

    const ursine::resources::ResourceReference &GetHeartBeatPlaySfx(void) const;
    void SetHeartBeatPlaySfx(const ursine::resources::ResourceReference &sfx);

    const ursine::resources::ResourceReference &GetHeartBeatStopSfx(void) const;
    void SetHeartBeatStopSfx(const ursine::resources::ResourceReference &sfx);

    const ursine::resources::ResourceReference &GetBossShieldDownSfx(void) const;
    void SetBossShieldDownSfx(const ursine::resources::ResourceReference &sfx);

    const ursine::Color &GetFlashColor(void) const;
    void SetFlashColor(const ursine::Color &color);

    float GetFlashDuration(void) const;
    void SetFlashDuration(float duration);

private:
    bool m_destroyed;

    ursine::resources::ResourceReference m_shieldBreakSfx;
    ursine::resources::ResourceReference m_shieldRebuildSfx;
    ursine::resources::ResourceReference m_hurtSfx;
    ursine::resources::ResourceReference m_heartBeatPlaySfx;
    ursine::resources::ResourceReference m_heartBeatStopSfx;
    ursine::resources::ResourceReference m_bossShieldDownSfx;

    float m_hurtSfxTimer;
    float m_hurtSfxMinDelay, m_hurtSfxMaxDelay;

    ursine::Color m_flashColor;

    bool m_playingHeartBeat;

    float m_duration;
    float m_timer;

    float m_initTimer;

    std::vector<ursine::ecs::ModelFragmenter*> m_fragmenters;
    std::vector<ursine::Color> m_defaultColors;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));
    void onHealthDamaged(EVENT_HANDLER(Health));
    void onBossShieldDown(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( Health )
    )
);
