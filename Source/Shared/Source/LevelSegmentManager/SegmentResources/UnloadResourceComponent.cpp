/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UnloadResourceComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "UnloadResourceComponent.h"

#include "LevelSegmentManagerComponent.h"

NATIVE_COMPONENT_DEFINITION( UnloadResource )

using namespace ursine;
using namespace ecs;

UnloadResource::UnloadResource(void)
    : BaseComponent( )
    , m_manager( nullptr )
    , m_unsubscribed( true )
    , m_unloadSegment( LevelSegments::Empty )
{
}

UnloadResource::~UnloadResource(void)
{
    auto world = GetOwner( )->GetWorld( );

    // If we haven't unsubscribed && the world isn't deleted
    if (!m_unsubscribed && world)
    {
        auto managers = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

        if (managers.size( ) == 0)
            return;

        m_manager->Listener( this )
            .Off( LevelSegmentManagerEvents::SegmentChanged, &UnloadResource::onSegmentChange );
        m_unsubscribed = true;
        m_manager = nullptr;
    }
}

UnloadResource::UnloadResource(LevelSegmentManager *manager, LevelSegments unloadSegment)
    : BaseComponent( )
    , m_unloadSegment( unloadSegment )
    , m_manager( manager )
    , m_unsubscribed( true )
{
    if (m_manager->GetCurrentSegment( ) >= m_unloadSegment)
        GetOwner( )->Delete( );
    else
    {
        m_manager->Listener( this )
            .On( LevelSegmentManagerEvents::SegmentChanged, &UnloadResource::onSegmentChange );

        m_unsubscribed = false;
    }
}

void UnloadResource::OnSceneReady(Scene *scene)
{
    if (m_manager)
        return;

    auto sm = GetOwner( )->GetWorld( )->GetEntitiesFromFilter(
        Filter( ).All<LevelSegmentManager>( ) 
    );

    if (sm.size( ) == 0)
        return;

    m_manager = sm[ 0 ]->GetComponent<LevelSegmentManager>( );

    if (m_manager->GetCurrentSegment( ) >= m_unloadSegment)
        GetOwner( )->Delete( );
    else
    {
        m_manager->Listener( this )
            .On( LevelSegmentManagerEvents::SegmentChanged, &UnloadResource::onSegmentChange );

        m_unsubscribed = false;
    }
}

LevelSegments UnloadResource::GetUnloadSegment(void) const
{
    return m_unloadSegment;
}

void UnloadResource::SetUnloadSegment(LevelSegments segment)
{
    m_unloadSegment = segment;
}

void UnloadResource::onSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    // Unload myself if we've changed to my unload segment
    if (m_unloadSegment == args->segment)
    {
        GetOwner( )->Delete( );
    }
}
