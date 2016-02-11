/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerAnimationSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class PlayerAnimationSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    PlayerAnimationSystem(ursine::ecs::World *world);

protected:
    void Process(ursine::ecs::Entity *entity) override;

} Meta(Enable);
