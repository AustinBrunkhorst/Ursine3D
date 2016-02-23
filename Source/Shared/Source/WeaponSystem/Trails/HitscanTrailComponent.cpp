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
#include "HitscanTrailComponent.h"
#include "ComponentIncludes.h"

NATIVE_COMPONENT_DEFINITION( HitscanTrailComponent ) ;

using namespace ursine;


HitscanTrailComponent::HitscanTrailComponent(void) :
    BaseComponent( ),
    m_vecToEnd( ),
    m_speed( 100.0f ),
    m_time( 0.0f ),
    m_timeToReachEnd( 2.0f ),
    m_segments( 4 )
{
}

HitscanTrailComponent::~HitscanTrailComponent(void)
{
}

// Time to reach end get / set
const float  HitscanTrailComponent::GetSpeed(void) const
{
    return m_speed;
}

void HitscanTrailComponent::SetSpeed(const float speed)
{
    m_speed = speed;
}


// Segments get / set
int HitscanTrailComponent::GetSegments(void) const
{
    return m_segments;
}

void HitscanTrailComponent::SetSegments(const int segments)
{
    m_segments = segments;
}


// particles to spawn get/set
int HitscanTrailComponent::GetParticlesToSpawn(void) const
{
    return m_particlesToSpawn;
}

void HitscanTrailComponent::SetParticlesToSpawn(const int count)
{
    m_particlesToSpawn = count;
}


// end settor 
void HitscanTrailComponent::SetVecToEnd(ursine::SVec3 end)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
{
    m_vecToEnd = end - GetOwner( )->GetTransform( )->GetWorldPosition( );

    // calc velocity
    m_velocity = m_speed * ursine::SVec3::Normalize(m_vecToEnd);

    // calc time to reach end
    m_timeToReachEnd = sqrt(m_vecToEnd.LengthSquared( ) / (m_velocity.LengthSquared( ) * 1000.0f) );
}


