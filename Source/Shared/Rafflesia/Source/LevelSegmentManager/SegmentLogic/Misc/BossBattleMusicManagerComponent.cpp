/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossBattleMusicManagerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossBattleMusicManagerComponent.h"

#include "LevelSegments.h"

#include <EntityEvent.h>
#include <AudioEventInfo.h>

NATIVE_COMPONENT_DEFINITION( BossBattleMusicManager );

using namespace ursine;
using namespace ecs;
using namespace resources;

BossBattleMusicManager::BossBattleMusicManager(void)
    : BaseComponent( ) { }

void BossBattleMusicManager::OnSceneReady(Scene *scene)
{
    auto world = GetOwner( )->GetWorld( );

    // play that shit
    auto emitter = GetOwner( )->GetComponent<AudioEmitter>( );

    emitter->PushEvent( m_startMusic );

    // subscribe to the level segment manager
    auto sm = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (sm.size( ))
    {
        auto manager = sm[ 0 ]->GetComponent<LevelSegmentManager>( );

        manager->Listener( this )
            .On( LevelSegmentManagerEvents::SegmentChanged, &BossBattleMusicManager::onSegmentChange );

        setCurrentState( manager->GetCurrentSegment( ) );
    }


    // subscribe to on death
    GetOwner( )->Listener( this )
        .On( ENTITY_REMOVED, &BossBattleMusicManager::onDeath );
}

const ResourceReference &BossBattleMusicManager::GetStartMusicEvent(void) const
{
    return m_startMusic;
}

void BossBattleMusicManager::SetStartMusicEvent(const ResourceReference &startMusicEvent)
{
    m_startMusic = startMusicEvent;

    NOTIFY_COMPONENT_CHANGED( "startMusicEvent", m_startMusic );
}

const ResourceReference &BossBattleMusicManager::GetStopMusicEvent(void) const
{
    return m_stopMusic;
}

void BossBattleMusicManager::SetStopMusicEvent(const ResourceReference &stopMusicEvent)
{
    m_stopMusic = stopMusicEvent;

    NOTIFY_COMPONENT_CHANGED( "stopMusicEvent", m_stopMusic );
}

void BossBattleMusicManager::onSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    setCurrentState( args->segment );
}

void BossBattleMusicManager::onDeath(EVENT_HANDLER(Entity))
{
    GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_stopMusic );
}

void BossBattleMusicManager::setCurrentState(LevelSegments segment)
{
    // change the state based on the segment
    auto emitter = GetOwner( )->GetComponent<AudioEmitter>( );
    auto event = std::make_shared<AudioState>( );

    switch (segment)
    {
    case LevelSegments::BossRoom_Platforming:
    case LevelSegments::BossRoom_Introduction:
        event->audioState = "Intro";
        break;
    case LevelSegments::BossRoom_Phase1:
        event->audioState = "Phase1";
        break;
    case LevelSegments::BossRoom_Phase2:
        event->audioState = "Phase2";
        break;
    case LevelSegments::BossRoom_Phase3:
        event->audioState = "Phase3";
        break;
    case LevelSegments::BossRoom_Phase4:
        event->audioState = "Phase4";
        break;
    case LevelSegments::BossRoom_Phase5:
        event->audioState = "Outro";
        break;
    default:
        return;
    }

    event->stateGroup = "Boss_Phases";

    emitter->PushEvent( event );
}
