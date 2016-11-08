/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorDebugDrawSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class EntityAnimatorDebugDrawSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    EntityAnimatorDebugDrawSystem(ursine::ecs::World *world);

private:
    void Process(const ursine::ecs::EntityHandle &entity) override;

} Meta(Enable, AutoAddEntitySystem);
