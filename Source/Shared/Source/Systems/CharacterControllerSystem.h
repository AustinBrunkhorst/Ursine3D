/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CharacterControllerSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class CharacterControllerSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    CharacterControllerSystem(ursine::ecs::World *world);

protected:

    void Process(ursine::ecs::Entity *entity);

} Meta(Enable);