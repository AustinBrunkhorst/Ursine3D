/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ControlPointSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "ControlPointSystem.h"
#include "SystemIncludes.h"
#include "ControlPointComponent.h"

ENTITY_SYSTEM_DEFINITION( ControlPointSystem );

ControlPointSystem::ControlPointSystem(ursine::ecs::World *world) 
    : FilterSystem( world, ursine::ecs::Filter( ).All<ControlPoint>( ) )
{ 
}

void ControlPointSystem::Begin(void)
{
    m_dt = ursine::Application::Instance->GetDeltaTime( );
}

void ControlPointSystem::Process(const ursine::ecs::EntityHandle &entity)
{
    ControlPoint& cp = *entity->GetComponent<ControlPoint>( );

    if ( cp.m_active )
    {
        cp.m_controlTimer -= m_dt;

        if ( cp.m_controlTimer <= 0.0f )
        {
            m_world->Dispatch(game::CONTROL_POINT_SUCCESS, ursine::EventArgs::Empty);
            entity->Delete( );
        }

        cp.m_active = false;
    }
}

