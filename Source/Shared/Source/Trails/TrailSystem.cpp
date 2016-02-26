
/* ----------------------------------------------------------------------------
** Team Bear King
** � 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TrailSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include <Application.h>
#include "TrailSystem.h"
#include "TrailComponent.h"
#include "ParticleEmitterComponent.h"

ENTITY_SYSTEM_DEFINITION( TrailSystem ) ;

using namespace ursine;
using namespace ursine::ecs;

namespace
{
    void CalculateEndPos(float elapsedTime, float endTime, const ursine::SVec3& dist, ursine::SVec3& movementVec)
    {
        movementVec = elapsedTime / endTime * dist;
    }

} // unnamed namespace


////////////////////////
////  Trail System  ////
////////////////////////

TrailSystem::TrailSystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).One< TrailComponent >( ) )
{
}


void TrailSystem::Enable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    m_particleEmitter[ uniqueID ] = entity->GetComponent< ParticleEmitter >( );
    
    m_trails[ uniqueID ] = entity->GetComponent< TrailComponent >( );

    m_transforms[ uniqueID ] = entity->GetTransform( );
}

void TrailSystem::Disable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    m_trails.erase( uniqueID );
    m_transforms.erase( uniqueID );
}

void TrailSystem::onUpdate(EVENT_HANDLER(World))
{
    for ( auto it : m_trails )
    {
        UpdateTrail(it.first, it.second);
    }
}


void TrailSystem::UpdateTrail(ursine::ecs::EntityUniqueID id, TrailComponent* const trail)
{
    if ( trail->m_distToTravel <= trail->m_distTraveled )
        trail->GetOwner( )->Delete( );

    // Calculate segment vec
    ursine::SVec3 segmentVec = trail->m_velocity * ( 1.0f / trail->m_segments );
    float segDist = segmentVec.Length( );

    // grab particle emitter for emitting particles at segments
    ParticleEmitter* const emitter = m_particleEmitter[ id ];

    // grab transform for modifying position
    Transform* const transform = m_transforms[ id ];

    ursine::SVec3 pos = transform->GetWorldPosition( );

    // emit particles at all segment positions
    for ( int i = 0; i < trail->m_segments && trail->m_distToTravel > trail->m_distTraveled; ++i, trail->m_distTraveled += segDist )
    {

        if ( trail->m_distTraveled > trail->m_distToTravel )
            break;

        pos += segmentVec;
        transform->SetWorldPosition( pos );

        // spawn all particles
        for ( int j = 0; j < trail->m_particlesToSpawn; ++j )
        {
            emitter->spawnParticle( );
        }
    }

}