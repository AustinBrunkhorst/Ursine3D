/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ChangeSegmentOnAnimatorFinishComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ChangeSegmentOnAnimatorFinishComponent.h"
#include "LevelSegmentManagerComponent.h"

NATIVE_COMPONENT_DEFINITION(ChangeSegmentOnAnimatorFinish);

using namespace ursine;
using namespace ecs;

ChangeSegmentOnAnimatorFinish::ChangeSegmentOnAnimatorFinish(void)
    : BaseComponent( )
    , m_destination( LevelSegments::Empty ) { }

ChangeSegmentOnAnimatorFinish::~ChangeSegmentOnAnimatorFinish(void)
{
    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

    if (animator)
    {
        animator->Listener( this )
            .Off( EntityAnimatorEvent::FinishedAnimating, &ChangeSegmentOnAnimatorFinish::onAnimationFinish );
    }
}

void ChangeSegmentOnAnimatorFinish::OnSceneReady(Scene *scene)
{
    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

    if (animator)
    {
        animator->Listener( this )
            .On( EntityAnimatorEvent::FinishedAnimating, &ChangeSegmentOnAnimatorFinish::onAnimationFinish );
    }
}

LevelSegments ChangeSegmentOnAnimatorFinish::GetDestinationSegment(void) const
{
    return m_destination;
}

void ChangeSegmentOnAnimatorFinish::SetDestinationSegment(LevelSegments destination)
{
    m_destination = destination;

    NOTIFY_COMPONENT_CHANGED( "destinationSegment", m_destination);
}

void ChangeSegmentOnAnimatorFinish::onAnimationFinish(EVENT_HANDLER(EntityAnimator))
{
    auto world = GetOwner( )->GetWorld( );

    if (world)
    {
        auto sm = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

        if (sm.size( ) == 0 || m_destination == LevelSegments::Empty )
            return;

        sm[ 0 ]->GetComponent<LevelSegmentManager>( )->SetCurrentSegment( m_destination );
    }
}
