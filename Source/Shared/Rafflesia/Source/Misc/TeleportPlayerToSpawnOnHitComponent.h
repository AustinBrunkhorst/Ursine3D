/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TeleportPlayerToSpawnOnHitComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <GhostComponent.h>

class TeleportPlayerToSpawnOnHit : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    TeleportPlayerToSpawnOnHit(void);
    ~TeleportPlayerToSpawnOnHit(void);

    void OnSceneReady(ursine::Scene *scene) override;

private:
    void onCollide(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( ursine::ecs::Ghost )
    )
);
