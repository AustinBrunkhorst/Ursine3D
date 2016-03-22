/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SegmentChangeTrigger.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "SegmentChangeTriggerComponent.h"
#include "PlayerIdComponent.h"
#include "LevelSegmentManagerComponent.h"
#include "GameEvents.h"
#include "GhostComponent.h"
#include <Core\CoreSystem.h>
#include <Filter.h>
#include <CollisionEventArgs.h>
#include <World.h>

NATIVE_COMPONENT_DEFINITION(SegmentChangeTrigger);


SegmentChangeTrigger::SegmentChangeTrigger(void)
    : BaseComponent( )
    , m_newSegment( LevelSegments_enum::Empty )
    , m_currentSegment( LevelSegments_enum::Empty )
    , triggerPlayer1( false )
    , triggerPlayer2( false )
    , m_collisionPlayer1( false )
    , m_collisionPlayer2( false )
    , m_unsubscribed( true )
{
}

SegmentChangeTrigger::~SegmentChangeTrigger(void)
{
    if (!m_unsubscribed)
        GetOwner( )->Listener( this )
            .Off( ursine::ecs::ENTITY_COLLISION_PERSISTED, &SegmentChangeTrigger::onCollision );
}

LevelSegments SegmentChangeTrigger::GetCurrentLevelSegment(void) const
{
    return m_currentSegment;
}

void SegmentChangeTrigger::SetCurrentLevelSegment(LevelSegments levelSegment)
{
    m_currentSegment = levelSegment;
}

LevelSegments SegmentChangeTrigger::GetNewLevelSegment(void) const
{
    return m_newSegment;
}

void SegmentChangeTrigger::SetNewLevelSegment(LevelSegments levelSegment)
{
    m_newSegment = levelSegment;
}

void SegmentChangeTrigger::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ursine::ecs::ENTITY_COLLISION_PERSISTED, &SegmentChangeTrigger::onCollision );

    m_unsubscribed = false;
}

void SegmentChangeTrigger::onCollision(EVENT_HANDLER(ursine::ecs::Entity))
{
    EVENT_ATTRS( ursine::ecs::Entity, ursine::physics::CollisionEventArgs );

    if (m_newSegment == LevelSegments::Empty)
        return;

    bool collisionPlayer1 = false, collisionPlayer2 = false, triggered = false;
    std::vector<ursine::physics::GhostOverlappingItem> items;

    if (args->thisEntity->HasComponent<ursine::ecs::Ghost>( ))
    {
        args->thisEntity->GetComponent<ursine::ecs::Ghost>( )->GetOverlappingPairs( items );
    }
    else
        return;

    for (auto object : items)
    {
        if (object.component->GetOwner( )->HasComponent<PlayerID>( ))
        {
            auto id = object.component->GetOwner( )->GetComponent<PlayerID>( )->GetID( );
            if (id == 0)
                collisionPlayer1 = true;
            else if (id == 1)
                collisionPlayer2 = true;
        }
    }
    // If players collide
    if (!triggerPlayer1 && !triggerPlayer2)
    {
        if (collisionPlayer1 || collisionPlayer2)
            triggered = true;
    }
    else if (triggerPlayer1 && triggerPlayer2)
    {
        if (collisionPlayer1 && collisionPlayer2)
            triggered = true;
    }
    else if (triggerPlayer1 && collisionPlayer1)
    {
        triggered = true;
    }
    else if (triggerPlayer2 && collisionPlayer2)
    {
        triggered = true;
    }

    if (triggered)
    {
        // Change the segment
        auto levelSegmentManagers = GetOwner( )->GetWorld( )->
            GetEntitiesFromFilter( ursine::ecs::Filter( ).All<LevelSegmentManager>( ) );

        UAssert(
            levelSegmentManagers.size( ),
            "Error: This component requires there to be"
            " a level segment manager in order to switch the segment"
        );

        // get [0] element
        auto stateMachine = levelSegmentManagers[ 0 ]->GetComponent<LevelSegmentManager>( );

        if (m_currentSegment != stateMachine->GetCurrentSegment( ))
            return;

        // call change state
        stateMachine->SetCurrentSegment( m_newSegment );

        if (!m_unsubscribed)
        {
            GetOwner( )->Listener( this )
                .Off( ursine::ecs::ENTITY_COLLISION_PERSISTED, &SegmentChangeTrigger::onCollision );

            m_unsubscribed = true;
        }

        // delete myself because this is a one time use ting
        GetOwner( )->Delete( );
    }

}
