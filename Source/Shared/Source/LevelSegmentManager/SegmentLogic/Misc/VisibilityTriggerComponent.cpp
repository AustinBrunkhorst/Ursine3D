/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** VisibilityTriggerComponent.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "VisibilityTriggerComponent.h"
#include "LevelSegmentManagerComponent.h"
#include "Model3DComponent.h"
#include <Core\CoreSystem.h>
#include <Filter.h>
#include <CollisionEventArgs.h>
#include <World.h>

NATIVE_COMPONENT_DEFINITION(VisibilityTrigger);

using namespace ursine;
using namespace ecs;

VisibilityTrigger::VisibilityTrigger(void)
    : BaseComponent()
    , m_segment(LevelSegments::Empty)
    , m_unsubscribed( true )
{
}

VisibilityTrigger::~VisibilityTrigger(void)
{
    if (!m_unsubscribed)
    {
        auto world = GetOwner( )->GetWorld( );

        if (!world)
            return;

        auto segmentManagers = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

        if (segmentManagers.size( ) == 0)
            return;

        auto segmentManager = segmentManagers[ 0 ];

        segmentManager->GetComponent<LevelSegmentManager>( )->Listener( this )
            .Off( LevelSegmentManagerEvents::SegmentChanged, &VisibilityTrigger::onSegmentChange );
    }

}

void VisibilityTrigger::OnInitialize(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto segmentManagers = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (segmentManagers.size( ) == 0)
        return;

    auto segmentManager = segmentManagers[ 0 ];

    segmentManager->GetComponent<LevelSegmentManager>( )->Listener( this )
        .On( LevelSegmentManagerEvents::SegmentChanged, &VisibilityTrigger::onSegmentChange );

    m_unsubscribed = false;
}

LevelSegments VisibilityTrigger::GetLevelSegment(void) const
{
    return m_segment;
}

void VisibilityTrigger::SetLevelSegment(LevelSegments levelSegment)
{
    m_segment = levelSegment;
}

void VisibilityTrigger::onSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    if (m_segment == LevelSegments::Empty)
        return;

    if (args->segment == m_segment)
    {
        if (GetOwner( )->HasComponent<Model3D>( ))
        {
            auto model = GetOwner( )->GetComponent<Model3D>( );
            auto flag = model->GetActive( );
            model->SetActive( !flag );

            auto world = GetOwner( )->GetWorld( );

            if (!world)
                return;

            auto segmentManagers = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

            if (segmentManagers.size( ) == 0)
                return;

            auto segmentManager = segmentManagers[ 0 ];

            segmentManager->GetComponent<LevelSegmentManager>( )->Listener( this )
                .Off( LevelSegmentManagerEvents::SegmentChanged, &VisibilityTrigger::onSegmentChange );

            m_unsubscribed = true;
        }
    }
}
