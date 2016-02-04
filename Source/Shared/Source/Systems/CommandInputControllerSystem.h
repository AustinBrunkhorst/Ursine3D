/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandInputControllerSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class CommandInputControllerSystem 
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    CommandInputControllerSystem(ursine::ecs::World *world);

protected:

    void Process(ursine::ecs::Entity *entity);

} Meta(Enable);