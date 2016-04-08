/* ---------------------------------------------------------------------------
** Team Bear King
** (c) 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GameEntryPointSystem.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>

class GameEntryPointSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    GameEntryPointSystem(ursine::ecs::World *world);

private:
    void OnInitialize(void) override;
    void OnSceneReady(ursine::Scene *scene) override;
    void OnRemove(void) override;

    void onGameplayStarted(EVENT_HANDLER(ursine::UIScreenManager));
} Meta(Enable);
