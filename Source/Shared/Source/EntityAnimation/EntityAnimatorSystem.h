/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class EntityAnimator;

class EntityAnimatorSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:

    EntityAnimatorSystem(ursine::ecs::World *world);

private:

    void Initialize(void) override;

    void Process(ursine::ecs::Entity *entity) override;

    void smoothUpdate(EntityAnimator *animator);

    void linearUpdate(EntityAnimator *animator);
} Meta(Enable, AutoAddEntitySystem);