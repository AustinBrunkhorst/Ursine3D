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

NATIVE_COMPONENT_DEFINITION( AnimateOnSegment );

using namespace ursine;
using namespace ecs;

AnimateOnSegment::AnimateOnSegment(void)
    : BaseComponent( )
    , m_segment( LevelSegments::Empty )
{
}

AnimateOnSegment::~AnimateOnSegment(void)
{
    connectToEvent( false );
}

void AnimateOnSegment::OnInitialize(void)
{
    connectToEvent( true );
}

LevelSegments AnimateOnSegment::GetLevelSegment(void) const
{
    return m_segment;
}

void AnimateOnSegment::SetLevelSegment(LevelSegments levelSegment)
{
    m_segment = levelSegment;
}

void AnimateOnSegment::onSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    if (m_segment == LevelSegments::Empty)
        return;

    if (args->segment == m_segment)
    {
        auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

        if (animator)
            animator->Play( );

        auto group = GetOwner( )->GetComponent<EntityAnimatorGroup>( );

        if (group)
            group->PlayGroupAnimators( );
    }
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
