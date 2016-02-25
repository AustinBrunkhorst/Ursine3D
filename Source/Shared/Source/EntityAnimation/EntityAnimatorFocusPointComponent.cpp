/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorFocusModifierComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EntityAnimatorFocusModifierComponent.h"

NATIVE_COMPONENT_DEFINITION( EntityAnimatorFocusModifier );

EntityAnimatorFocusModifier::EntityAnimatorFocusModifier(void)
    : BaseComponent( )
    , m_focusEntity( "" )
{
}

EntityAnimatorFocusModifier::~EntityAnimatorFocusModifier(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (world)
        world->Listener( this )
            .Off( ursine::ecs::WORLD_UPDATE, &EntityAnimatorFocusModifier::onUpdate );
}


const std::string &EntityAnimatorFocusModifier::GetFocusEntity(void) const
{
    return m_focusEntity;
}

void EntityAnimatorFocusModifier::SetFocusEntity(const std::string &focus)
{
    m_focusEntity = focus;
}

void EntityAnimatorFocusModifier::OnInitialize(void)
{
    auto world = GetOwner( )->GetWorld( );

    world->Listener( this )
        .On( ursine::ecs::WORLD_UPDATE, &EntityAnimatorFocusModifier::onUpdate, 100 );
}

void EntityAnimatorFocusModifier::onUpdate(EVENT_HANDLER(World))
{
    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto entity = world->GetEntityFromName( m_focusEntity );

    if (!entity)
        return;

    auto pos = entity->GetTransform( )->GetWorldPosition( );

    GetOwner( )->GetTransform( )->LookAt( pos );
}
