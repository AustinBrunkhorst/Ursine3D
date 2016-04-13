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

    PlayerDownedObject(void);
    ~PlayerDownedObject(void);

    const ursine::resources::ResourceReference& GetReviveObject(void) const;
    void SetReviveObject(const ursine::resources::ResourceReference& archetype);

private:

    void OnInitialize(void) override;

    void onHeirarchy(EVENT_HANDLER(ursine::ecs::Entity));
    void onZeroHealth(EVENT_HANDLER(ursine::ecs::Entity));
    void onRevive(EVENT_HANDLER(ursine::ecs::Entity));
    void onCollision(EVENT_HANDLER(ursine::ecs::Entity));

    ursine::resources::ResourceReference m_reviveObject;

} Meta(
    Enable,
    DisplayName( "PlayerDownedObject" )
) EditorMeta(
    RequiresComponents(
        typeof( Health )
    )
);
