/* ----------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnerGroupSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>

class SpawnerGroupSystem : ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    
    SpawnerGroupSystem(ursine::ecs::World *world);

private:

    void OnInitialize(void) override;
    void OnRemove(void) override;

    void onUpdate(EVENT_HANDLER(World));

    void onComponentAdded(EVENT_HANDLER(World));
    void onComponentRemoved(EVENT_HANDLER(World));

} Meta(Enable);

