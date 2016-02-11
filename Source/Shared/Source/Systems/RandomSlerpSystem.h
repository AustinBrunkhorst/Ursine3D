/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RandomSlerpSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class RandomSlerpSystem 
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    RandomSlerpSystem(ursine::ecs::World *world);

protected:
    void Process(ursine::ecs::Entity *entity) override;

} Meta(Enable);
