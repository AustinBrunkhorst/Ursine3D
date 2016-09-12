/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SeedshotProjectileComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <AudioItemEventData.h>
#include <ArchetypeData.h>

#include <AudioEmitterComponent.h>

class SeedshotProjectile : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorResourceField(
        ursine::resources::AudioItemEventData,
        seedshotSfx,
        GetSeedshotSfx,
        SetSeedshotSfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        seedshotEndSfx,
        GetSeedshotEndSfx,
        SetSeedshotEndSfx
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        damageSfxEmitter,
        GetDamageSfxEmitter,
        SetDamageSfxEmitter
    );

    SeedshotProjectile(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const ursine::resources::ResourceReference &GetSeedshotSfx(void) const;
    void SetSeedshotSfx(const ursine::resources::ResourceReference &seedshotSfx);

    const ursine::resources::ResourceReference &GetSeedshotEndSfx(void) const;
    void SetSeedshotEndSfx(const ursine::resources::ResourceReference &seedshotEndSfx);

    const ursine::resources::ResourceReference &GetDamageSfxEmitter(void) const;
    void SetDamageSfxEmitter(const ursine::resources::ResourceReference &damageSfxEmitter);

private:
    ursine::resources::ResourceReference m_seedshotSfx;
    ursine::resources::ResourceReference m_seedshotEndSfx;
    ursine::resources::ResourceReference m_damageSfxEmitter;

    void onDeath(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( ursine::ecs::AudioEmitter )
    )
);
