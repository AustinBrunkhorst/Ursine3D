/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ControlPointSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/
#pragma once

#include <FilterSystem.h>

class ControlPointSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;
public:

    ControlPointSystem(ursine::ecs::World * world);
    void Begin(void) override;
    void Process(ursine::ecs::Entity* entity) override;

private:

    float m_dt;

} Meta(Enable);