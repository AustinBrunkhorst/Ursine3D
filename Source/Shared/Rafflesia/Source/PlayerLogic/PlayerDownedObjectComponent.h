/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerDownedObjectComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <AudioItemEventData.h>

#include "HealthComponent.h"


class PlayerDownedObject : public ursine::ecs::Component
{
    NATIVE_COMPONENT

public:

    EditorButton(
        sendReviveEvent,
        "Send Revive Event [Debugging]"
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        ReviveObject,
        GetReviveObject,
        SetReviveObject
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        deathSfx,
        GetDeathSfx,
        SetDeathSfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        whileDeadSfx,
        GetWhileDeadSfx,
        SetWhileDeadSfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        notDeadSfx,
        GetNotDeadSfx,
        SetNotDeadSfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        beingRevivedSfx,
        GetBeingRevivedSfx,
        SetBeingRevivedSfx
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        revivingSfx,
        GetRevivingSfx,
        SetRevivingSfx
    );


    PlayerDownedObject(void);
    ~PlayerDownedObject(void);

    const ursine::resources::ResourceReference& GetReviveObject(void) const;
    void SetReviveObject(const ursine::resources::ResourceReference& archetype);

    const ursine::resources::ResourceReference &GetDeathSfx(void) const;
    void SetDeathSfx(const ursine::resources::ResourceReference &sfx);

    const ursine::resources::ResourceReference &GetWhileDeadSfx(void) const;
    void SetWhileDeadSfx(const ursine::resources::ResourceReference &sfx);

    const ursine::resources::ResourceReference &GetNotDeadSfx(void) const;
    void SetNotDeadSfx(const ursine::resources::ResourceReference &sfx);

    const ursine::resources::ResourceReference &GetBeingRevivedSfx(void) const;
    void SetBeingRevivedSfx(const ursine::resources::ResourceReference &sfx);

    const ursine::resources::ResourceReference &GetRevivingSfx(void) const;
    void SetRevivingSfx(const ursine::resources::ResourceReference &sfx);

private:

    void OnInitialize(void) override;

    void onHeirarchy(EVENT_HANDLER(ursine::ecs::Entity));
    void onZeroHealth(EVENT_HANDLER(ursine::ecs::Entity));
    void onRevive(EVENT_HANDLER(ursine::ecs::Entity));
    void onCollision(EVENT_HANDLER(ursine::ecs::Entity));

    ursine::resources::ResourceReference m_reviveObject;

    ursine::resources::ResourceReference m_deathSfx;
    ursine::resources::ResourceReference m_whileDeadSfx;
    ursine::resources::ResourceReference m_notDeadSfx;

    ursine::resources::ResourceReference m_beingRevivedSfx;
    ursine::resources::ResourceReference m_revivingSfx;

} Meta(
    Enable,
    DisplayName( "PlayerDownedObject" )
) EditorMeta(
    RequiresComponents(
        typeof( Health )
    )
);
