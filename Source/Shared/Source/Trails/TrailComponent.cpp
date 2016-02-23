/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HitscanTrailComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "TrailComponent.h"
#include "ComponentIncludes.h"

NATIVE_COMPONENT_DEFINITION( TrailComponent ) ;

using namespace ursine;


TrailComponent::HitscanTrailComponent(void) :
    BaseComponent( ),
    m_vecToEnd( ),
    m_speed( 100.0f ),
    m_time( 0.0f ),
    m_timeToReachEnd( 2.0f ),
    m_segments( 4 )
{
}

TrailComponent::~TrailComponent(void)
{
}

// Time to reach end get / set
const float  TrailComponent::GetSpeed(void) const
{
    return m_speed;
}

void TrailComponent::SetSpeed(const float speed)
{
    m_speed = speed;
}


// Segments get / set
int TrailComponent::GetSegments(void) const
{
    return m_segments;
}

void TrailComponent::SetSegments(const int segments)
{
    m_segments = segments;
}


// particles to spawn get/set
int TrailComponent::GetParticlesToSpawn(void) const
{
    return m_particlesToSpawn;
}

void TrailComponent::SetParticlesToSpawn(const int count)
{
    m_particlesToSpawn = count;
}


// end settor 
void TrailComponent::SetVecToEnd(ursine::SVec3 end)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
{
    m_vecToEnd = end - GetOwner( )->GetTransform( )->GetWorldPosition( );

    // calc velocity
    m_velocity = m_speed * ursine::SVec3::Normalize(m_vecToEnd);

    // calc time to reach end
    m_timeToReachEnd = sqrt(m_vecToEnd.LengthSquared( ) / (m_velocity.LengthSquared( ) * 1000.0f) );
}


