/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimateEntityOnTriggerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "AnimateEntityOnTriggerComponent.h"

#include "PlayerIDComponent.h"
#include "EntityAnimatorComponent.h"
#include "EntityAnimatorGroupComponent.h"

#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION(AnimateEntityOnTrigger);

AnimateEntityOnTrigger::AnimateEntityOnTrigger(void)
    : BaseComponent( )
    , m_destroyOnPlay( true )
    , m_unsubscribed( true ) { }

AnimateEntityOnTrigger::~AnimateEntityOnTrigger(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (world && !m_unsubscribed)
        world->Listener( this )
            .Off( WORLD_UPDATE, &AnimateEntityOnTrigger::onUpdate );
}

const std::string &AnimateEntityOnTrigger::GetEntityName(void) const
{
    return m_name;
}

void AnimateEntityOnTrigger::SetEntityName(const std::string &name)
{
    m_name = name;

    NOTIFY_COMPONENT_CHANGED( "entityName", m_name );
}

bool AnimateEntityOnTrigger::GetDestroyOnPlay(void) const
{
    return m_destroyOnPlay;
}

void AnimateEntityOnTrigger::SetDestroyOnPlay(bool flag)
{
    m_destroyOnPlay = flag;

    NOTIFY_COMPONENT_CHANGED( "destroyOnPlay", m_destroyOnPlay );
}

void AnimateEntityOnTrigger::OnSceneReady(Scene *scene)
{
    auto world = GetOwner( )->GetWorld( );

    // find the entity
    m_entity = world->GetEntityFromName( m_name );

    // subscribe to my collision events
    world->Listener( this )
        .On( WORLD_UPDATE, &AnimateEntityOnTrigger::onUpdate );

    m_unsubscribed = false;
}

void AnimateEntityOnTrigger::onUpdate(EVENT_HANDLER(World))
{
    // get the ghost collider
    auto ghost = GetOwner( )->GetComponent<Ghost>( );
    std::vector<physics::GhostOverlappingItem> items;

    ghost->GetOverlappingPairs( items );

    for (auto &obj : items)
    {
        if (obj.component->GetOwner( )->HasComponent<PlayerID>( ))
        {
            // We're playin folks
            if (m_entity)
            {
                auto entityAnimator = m_entity->GetComponent<EntityAnimator>( );
                auto entityGroup = m_entity->GetComponent<EntityAnimatorGroup>( );

                if (entityAnimator)
                    entityAnimator->Play( );
                if (entityGroup)
                    entityGroup->PlayGroupAnimators( );
            }

            // unsubscribe from update
            GetOwner( )->GetWorld( )->Listener( this )
                .Off( WORLD_UPDATE, &AnimateEntityOnTrigger::onUpdate );

            m_unsubscribed = true;

            // delete myself
            if (m_destroyOnPlay)
                GetOwner( )->Delete( );

            return;
        }
    }
}
