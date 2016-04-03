/*---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LightingGroupComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LightingGroupComponent.h"

#include <EntityEvent.h>
#include <LightComponent.h>
#include <ParticleEmitterComponent.h>

NATIVE_COMPONENT_DEFINITION(LightingGroup);

using namespace ursine;
using namespace ecs;

LightingGroup::LightingGroup(void)
    : BaseComponent( )
    , m_initiallyActive( false ) { }

void LightingGroup::OnSceneReady(Scene *scene)
{
    // subscribe to the hierarchy serialization event
    GetOwner( )->Listener( this )
        .On( ENTITY_HIERARCHY_SERIALIZED, &LightingGroup::onHierarchySerialized );
}

bool LightingGroup::GetInitiallyActive(void) const
{
    return m_initiallyActive;
}

void LightingGroup::SetInitiallyActive(bool active)
{
    m_initiallyActive = active;

    NOTIFY_COMPONENT_CHANGED( "initiallyActive", m_initiallyActive );
}

void LightingGroup::ToggleGroupLights(bool toggle)
{
    auto lights = GetOwner( )->GetComponentsInChildren<Light>( );

    for (auto &light : lights)
    {
        light->SetIsActive( toggle );
    }

    auto emitters = GetOwner( )->GetComponentsInChildren<ParticleEmitter>( );

    for (auto &emitter : emitters)
    {
        emitter->SetEmitting( toggle );
    }
}

void LightingGroup::onHierarchySerialized(EVENT_HANDLER(Entity))
{
    // unsubscribe to the hierarchy serialization event
    GetOwner( )->Listener( this )
        .Off( ENTITY_HIERARCHY_SERIALIZED, &LightingGroup::onHierarchySerialized );

    ToggleGroupLights( m_initiallyActive );
}
