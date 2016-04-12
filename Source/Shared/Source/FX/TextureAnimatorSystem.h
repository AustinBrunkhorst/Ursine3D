/*---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureAnimatorSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class TextureAnimatorSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    TextureAnimatorSystem(ursine::ecs::World *world);

private:
    void Process(const ursine::ecs::EntityHandle &entity) override;

} Meta(Enable, AutoAddEntitySystem);