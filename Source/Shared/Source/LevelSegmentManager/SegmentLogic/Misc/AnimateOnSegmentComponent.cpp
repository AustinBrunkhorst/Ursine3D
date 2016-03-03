/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimateOnSegmentComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "AnimateOnSegmentComponent.h"

#include "LevelSegmentManagerComponent.h"
#include "EntityAnimatorGroupComponent.h"

#include <EntityEvent.h>

NATIVE_COMPONENT_DEFINITION( AnimateOnSegment );

using namespace ursine;
using namespace ecs;

AnimateOnSegment::AnimateOnSegment(void)
    : BaseComponent( )
    , m_segment( LevelSegments::Empty )
    , m_greaterThan( false )
{
}

AnimateOnSegment::~AnimateOnSegment(void)
{
    connectToEvent( false );
}

void AnimateOnSegment::OnInitialize(void)
{
    connectToEvent( true );

    GetOwner( )->Listener( this )
        .On( ENTITY_HIERARCHY_SERIALIZED, &AnimateOnSegment::onHierarchySerialize );
}

LevelSegments AnimateOnSegment::GetLevelSegment(void) const
{
    return m_segment;
}

void AnimateOnSegment::SetLevelSegment(LevelSegments levelSegment)
{
    m_segment = levelSegment;
}

bool AnimateOnSegment::GetWhenGreaterThan(void) const
{
    return m_greaterThan;
}

void AnimateOnSegment::SetWhenGreaterThan(bool flag)
{
    m_greaterThan = flag;
}

void AnimateOnSegment::onHierarchySerialize(EVENT_HANDLER(ursine::ecs::Entity))
{
    auto segmentManagers = GetOwner( )->GetWorld( )->GetEntitiesFromFilter( 
        Filter( ).All<LevelSegmentManager>( ) 
    );

    if (segmentManagers.size( ))
    {
        auto manager = segmentManagers[ 0 ]->GetComponent<LevelSegmentManager>( );

        auto currentSegment = manager->GetCurrentSegment( );

        if (currentSegment >= m_segment)
            playAnimation( );
    }

    GetOwner( )->Listener( this )
        .Off( ENTITY_HIERARCHY_SERIALIZED, &AnimateOnSegment::onHierarchySerialize );
}

void AnimateOnSegment::onSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    if (m_segment == LevelSegments::Empty)
        return;

    if (args->segment >= m_segment)
        playAnimation( );
}

void AnimateOnSegment::connectToEvent(bool connect)
{
    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto segmentManagers = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (segmentManagers.size( ) == 0)
        return;

    auto segmentManager = segmentManagers[ 0 ];

    if (connect)
        segmentManager->GetComponent<LevelSegmentManager>( )->Listener( this )
            .On( LevelSegmentManagerEvents::SegmentChanged, &AnimateOnSegment::onSegmentChange );
    else
        segmentManager->GetComponent<LevelSegmentManager>( )->Listener( this )
            .Off( LevelSegmentManagerEvents::SegmentChanged, &AnimateOnSegment::onSegmentChange );
}

void AnimateOnSegment::playAnimation(void)
{
    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

    if (animator)
        animator->Play( );

    auto group = GetOwner( )->GetComponent<EntityAnimatorGroup>( );

    if (group)
        group->PlayGroupAnimators( );
}
