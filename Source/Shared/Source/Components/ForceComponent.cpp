/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ForceComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include <ForceComponent.h>
#include "ComponentIncludes.h"
#include <RigidbodyComponent.h>

NATIVE_COMPONENT_DEFINITION( ForceComponent ) ;

using namespace ursine;


ForceComponent::ForceComponent(void) :
    BaseComponent( ),
    m_force(0, 0, 0)
{
}

ForceComponent::~ForceComponent(void)
{
    GetOwner( )->Listener(this)
        .Off(ursine::ecs::ENTITY_COLLISION_PERSISTED, &ForceComponent::OnCollide);
}

const ursine::SVec3& ForceComponent::GetForce(void) const
{
    return m_force;
}

void ForceComponent::SetForce(const ursine::SVec3& force)
{
    m_force = force;
}

void ForceComponent::OnInitialize(void)
{
    GetOwner( )->Listener(this)
        .On(ursine::ecs::ENTITY_COLLISION_PERSISTED, &ForceComponent::OnCollide);
}

void ForceComponent::OnCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED))
{
    EVENT_ATTRS(ursine::ecs::Entity, ursine::physics::CollisionEventArgs);

    if ( args->otherEntity->HasComponent<ursine::ecs::Rigidbody>( ) )
    {
        args->otherEntity->GetComponent<ursine::ecs::Rigidbody>( )->AddImpulse( m_force );
    }

}


