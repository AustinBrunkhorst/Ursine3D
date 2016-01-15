#pragma once

/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TimeDeathSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <FilterSystem.h>

class TimeDeath;

class TimeDeathSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    TimeDeathSystem( ursine::ecs::World* world );

protected:
    void Process( ursine::ecs::Entity* entity );


} Meta( Enable );


