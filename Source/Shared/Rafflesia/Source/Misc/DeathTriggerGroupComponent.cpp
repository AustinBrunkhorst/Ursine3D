/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DeathTriggerGroupComponent.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "DeathTriggerGroupComponent.h"

#include "LevelSegmentManagerComponent.h"
#include "HealthComponent.h"

NATIVE_COMPONENT_DEFINITION(DeathTriggerGroup);

using namespace ursine;
using namespace ecs;

DeathTriggerGroup::DeathTriggerGroup(void)
    : BaseComponent( )
    , m_newSegment( LevelSegments::Empty )
    , m_numAlive( 0 )
{
}

LevelSegments DeathTriggerGroup::GetNewLevelSegment(void) const
{
    return m_newSegment;
}
void DeathTriggerGroup::SetNewLevelSegment(LevelSegments levelSegment)
{
    m_newSegment = levelSegment;
}

void DeathTriggerGroup::OnInitialize(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &DeathTriggerGroup::onUpdate );
}

void DeathTriggerGroup::onUpdate(EVENT_HANDLER(World))
{
    auto healthChildren = GetOwner( )->GetComponentsInChildren<Health>( );

    m_numAlive = static_cast<int>(healthChildren.size( ));

    for (auto &health : healthChildren)
    {
        health->Listener( this )
            .On( HEALTH_ZERO, &DeathTriggerGroup::onTargetDead );
    }

    GetOwner( )->GetWorld( )->Listener( this )
        .Off( WORLD_UPDATE, &DeathTriggerGroup::onUpdate );
}

void DeathTriggerGroup::onTargetDead(EVENT_HANDLER(Health))
{
    EVENT_ATTRS(Health, HealthEventArgs);

    --m_numAlive;

    if (m_numAlive <= 0)
    {
        auto levelSegmentManagers = GetOwner( )->GetWorld( )->
            GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

        UAssert(
            levelSegmentManagers.size( ),
            "Error: This component requires there to be"
            " a level segment manager in order to switch the segment"
            );

        // get [0] element
        auto stateMachine = levelSegmentManagers[ 0 ]->GetComponent<LevelSegmentManager>( );

        // call change state
        stateMachine->SetCurrentSegment( m_newSegment );

        // delete myself because this is a one time use ting
        GetOwner( )->Delete( );
    }
}
