/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Phase3LightTriggerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "Phase3LightTriggerComponent.h"
#include "PlayerIDComponent.h"

#include <Scene.h>

using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION(Phase3LightTrigger);

Phase3LightTrigger::Phase3LightTrigger(void)
    : BaseComponent( )
    , m_animated( false ) { }

Phase3LightTrigger::~Phase3LightTrigger(void)
{
    // unsubscribe from update
    auto world = GetOwner( )->GetWorld( );

    if (world)
        world->Listener( this )
            .Off( WORLD_UPDATE, &Phase3LightTrigger::onUpdate );
}

const std::string &Phase3LightTrigger::GetLeftSideLights(void) const
{
    return m_left;
}

void Phase3LightTrigger::SetLeftSideLights(const std::string &entity)
{
    m_left = entity;

    NOTIFY_COMPONENT_CHANGED( "leftSideLights", m_left );
}

const std::string &Phase3LightTrigger::GetRightSideLights(void) const
{
    return m_right;
}

void Phase3LightTrigger::SetRightSideLights(const std::string &entity)
{
    m_right = entity;

    NOTIFY_COMPONENT_CHANGED( "rightSideLights", m_right );
}

void Phase3LightTrigger::OnSceneReady(Scene *scene)
{
#if defined(URSINE_WITH_EDITOR)

    if (scene->GetPlayState( ) == PS_EDITOR)
        return;

#endif

    auto world = GetOwner( )->GetWorld( );

    world->Listener( this )
        .On( WORLD_UPDATE, &Phase3LightTrigger::onUpdate );

    auto leftEntity = world->GetEntityFromName( m_left );
    auto rightEntity = world->GetEntityFromName( m_right );

    UAssert( leftEntity && rightEntity, "Error: One of these entities does not exist." );

    m_leftGroup = leftEntity->GetComponent<LightingGroup>( );
    m_rightGroup = rightEntity->GetComponent<LightingGroup>( );

    UAssert( m_leftGroup && m_rightGroup, "Error: Both must have a lighting group component." );

    GetOwner( )->GetComponent<EntityAnimator>( )->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &Phase3LightTrigger::onAnimationFinished );
}

void Phase3LightTrigger::onUpdate(EVENT_HANDLER(ursine::ecs::World))
{
    if (!m_animated)
        return;

    auto ghost = GetOwner( )->GetComponent<Ghost>( );

    if (!ghost)
        return;

    std::vector<physics::GhostOverlappingItem> pairs;

    ghost->GetOverlappingPairs( pairs );

    int numPlayers = 0;

    for (auto &pair : pairs)
    {
        if (pair.component->GetOwner( )->GetRoot( )->HasComponent<PlayerID>( ))
            ++numPlayers;
    }

    switch (numPlayers)
    {
    case 0:
        m_leftGroup->ToggleGroupLights( false );
        m_rightGroup->ToggleGroupLights( false );
        break;
    case 1:
        m_leftGroup->ToggleGroupLights( true );
        m_rightGroup->ToggleGroupLights( false );
        break;
    case 2:
        m_leftGroup->ToggleGroupLights( true );
        m_rightGroup->ToggleGroupLights( true );

        // send event, delete myself and unsubscribe
        auto world = GetOwner( )->GetWorld( );

        world->Dispatch( BOTH_SIDES_TRIGGERED, nullptr );

        world->Listener( this )
            .Off( WORLD_UPDATE, &Phase3LightTrigger::onUpdate );

        GetOwner( )->Delete( );

        break;
    }
}

void Phase3LightTrigger::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    EVENT_SENDER(EntityAnimator, sender);

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &Phase3LightTrigger::onAnimationFinished );

    m_animated = true;
}
