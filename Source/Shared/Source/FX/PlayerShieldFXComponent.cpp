/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerShieldFXComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerShieldFXComponent.h"

#include "ShieldFXComponent.h"

#include <World.h>

NATIVE_COMPONENT_DEFINITION( PlayerShieldFX );

using namespace ursine;
using namespace ecs;

PlayerShieldFX::PlayerShieldFX(void)
    : BaseComponent( )
    , m_destroyed( false ) { }

PlayerShieldFX::~PlayerShieldFX(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (world)
    {
        world->Listener( this )
            .Off( WORLD_UPDATE, &PlayerShieldFX::onUpdate );
    }
}

void PlayerShieldFX::OnSceneReady(ursine::Scene *scene)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &PlayerShieldFX::onUpdate );
}

void PlayerShieldFX::onUpdate(EVENT_HANDLER(World))
{
    auto health = GetOwner( )->GetComponent<Health>( );

    auto shield = health->GetShieldHealth( );

    if (shield > 0.0f && m_destroyed)
    {
        auto fxs = GetOwner( )->GetComponentsInChildren<ShieldFX>( );

        for (auto &fx : fxs)
        {
            fx->RebuildShield( );
        }

        m_destroyed = false;
    }
    else if (shield == 0.0f && !m_destroyed)
    {
        auto fxs = GetOwner( )->GetComponentsInChildren<ShieldFX>( );

        for (auto &fx : fxs)
        {
            fx->DestroyShield( );
        }

        m_destroyed = true;
    }
}
