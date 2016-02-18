/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InputControllerSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class InputControllerSystem 
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    InputControllerSystem(ursine::ecs::World *world);

protected:

    void Process(ursine::ecs::Entity *entity);

} Meta(Enable);