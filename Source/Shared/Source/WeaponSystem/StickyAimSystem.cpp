
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** StickyAimSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "StickyAimSystem.h"
#include "RaycastComponent.h"
#include "CharacterControllerComponent.h"
#include "Application.h"
#include "Raycasting.h"
#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "AIHordelingTypeComponent.h"


ENTITY_SYSTEM_DEFINITION( StickyAimSystem ) ;

using namespace ursine;
using namespace ursine::ecs;

namespace
{
} // unnamed namespace


//////////////////////////////
////  Base Weapon System  ////
//////////////////////////////

StickyAimSystem::StickyAimSystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).All< RaycastComponent, CharacterController >( ) )
{
}

void StickyAimSystem::Initialize(void)
{
    m_physicsSystem = m_world->GetEntitySystem<PhysicsSystem>( );
}


void StickyAimSystem::Enable(const ursine::ecs::EntityHandle& entity)
{
    m_raycastComps[ entity ] = entity->GetComponent< RaycastComponent >( );
    m_charControllers[ entity ] = entity->GetComponent< CharacterController >( );
}

void StickyAimSystem::Disable(const ursine::ecs::EntityHandle& entity)
{
    m_raycastComps.erase( entity );
    m_charControllers.erase( entity );
}   

void StickyAimSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = ursine::Application::Instance->GetDeltaTime( );

    physics::RaycastInput  rayin;

    RaycastComponent* raycastComp;
    Transform* transComp;

    EntityHandle entity;

    for ( auto rayComps : m_raycastComps )
    {
        raycastComp = rayComps.second;
        transComp   = raycastComp->GetCameraTransform( );

        // setup rayin
        rayin.start = transComp ->GetWorldPosition( );
        rayin.end = transComp->GetWorldPosition( ) + transComp->GetForward( ) * raycastComp->GetRange( );

        physics::RaycastOutput rayout;

        if ( m_physicsSystem->Raycast(rayin, rayout, physics::RAYCAST_CLOSEST_NON_GHOST, raycastComp->GetDebug( ), 10.0f, Color(1.0f, 0.0f, 0.0f, 1.0f), false) )
        {
            entity = m_world->GetEntity( rayout.entity[ 0 ] );

            // if enemy then apply a modifier to players rotation
            if ( entity->GetRoot( )->HasComponent< AIHordelingType >( ) )
                m_charControllers[ rayComps.first ]->SetRotationModifier( raycastComp->GetRotationModifier( ) );

            else
                m_charControllers[ rayComps.first ]->SetRotationModifier( 1.0f );
        }

        else
            m_charControllers[ rayComps.first ]->SetRotationModifier( 1.0f );
    }
}
