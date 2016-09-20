/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RaycastComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "RaycastComponent.h"
#include "ComponentIncludes.h"
#include "CameraComponent.h"
#include "EntityEvent.h"

NATIVE_COMPONENT_DEFINITION( RaycastComponent ) ;

using namespace ursine;


RaycastComponent::RaycastComponent(void) :
    BaseComponent( ),
    m_range( 100.0f ),
    m_rotationModifier( 1.0f ),
    m_debug( false )
{
}

RaycastComponent::~RaycastComponent(void)
{
    GetOwner( )->Listener(this)
        .Off(ursine::ecs::ENTITY_HIERARCHY_SERIALIZED, &RaycastComponent::OnHeirachyFinished);
}

float RaycastComponent::GetRange(void) const
{
    return m_range;
}

void RaycastComponent::SetRange(float range)
{
    m_range = range;
}

float RaycastComponent::GetRotationModifier(void) const
{
    return m_rotationModifier;
}

void RaycastComponent::SetRotationModifier(float modifier)
{
    m_rotationModifier = modifier;
}

const ursine::ecs::EntityHandle& RaycastComponent::GetEntityHit(void) const
{
    return m_entityHit;
}

void RaycastComponent::SetEntityHit(ursine::ecs::EntityHandle& entity)
{
    m_entityHit = entity;
}

ursine::ecs::Transform* RaycastComponent::GetCameraTransform(void) const
{
    return m_camTransform;
}

bool RaycastComponent::GetDebug(void) const
{
    return m_debug;
}

void RaycastComponent::SetDebug(bool debug)
{
    m_debug = debug;
}

void RaycastComponent::OnInitialize(void)
{
    GetOwner( )->Listener(this)
        .On( ursine::ecs::ENTITY_HIERARCHY_SERIALIZED, &RaycastComponent::OnHeirachyFinished );
}

void RaycastComponent::OnHeirachyFinished(EVENT_HANDLER(ursine::ecs::Entity))
{
    auto cameraComp = GetOwner( )->GetComponentInChildren< ursine::ecs::Camera >( );

    if ( cameraComp != nullptr )
        m_camTransform = cameraComp->GetOwner( )->GetTransform( );
    else
        m_camTransform = GetOwner( )->GetTransform( );
}

