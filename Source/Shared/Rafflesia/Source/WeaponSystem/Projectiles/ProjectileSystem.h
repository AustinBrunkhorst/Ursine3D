/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectileSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class ProjectileSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;
public:
    ProjectileSystem( ursine::ecs::World* world );

protected:
    void Process(const ursine::ecs::EntityHandle &entity ) override;

private:

} Meta (Enable);