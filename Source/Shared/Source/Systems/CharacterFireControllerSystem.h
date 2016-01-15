/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CharacterFireControllerSystem.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#pragma once

#include <FilterSystem.h>
#include <Randomizer.h>

class CharacterFireControllerSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    CharacterFireControllerSystem(ursine::ecs::World *world);

protected:
    void Process(ursine::ecs::Entity *entity) override;
	
    ursine::Randomizer m_rng;

} Meta(Enable);