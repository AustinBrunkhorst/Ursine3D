/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ControlPointComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "ControlPointComponent.h"
#include "PlayerIdComponent.h"
#include "GameEvents.h"
#include <Core\CoreSystem.h>
#include <CollisionEventArgs.h>
#include <World.h>

NATIVE_COMPONENT_DEFINITION( ControlPoint ) ;


ControlPoint::ControlPoint( void )
    : BaseComponent( )
    , m_controlTimer( 10.0f )
    , m_active( false )
{
}

ControlPoint::~ControlPoint(void)
{
    GetOwner( )->Listener( this )
        .Off(ursine::ecs::ENTITY_COLLISION_PERSISTED, &ControlPoint::Activate);
}

void ControlPoint::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On(ursine::ecs::ENTITY_COLLISION_PERSISTED, &ControlPoint::Activate);
}



float ControlPoint::GetControlTimer( void ) const
{
    return m_controlTimer;
}

void ControlPoint::SetControlTimer(const float time)
{
    m_controlTimer = time;
}

void ControlPoint::Activate(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED))
{
    EVENT_ATTRS(ursine::ecs::Entity, ursine::physics::CollisionEventArgs);

    if ( args->otherEntity->HasComponent<PlayerID>( ) )
    {
        m_active = true;
    }

}
