/*---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShieldFXComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ShieldFXSystem.h"
#include "ShieldFXComponent.h"
#include "FragmentationComponent.h"

#include <Application.h>
#include <Scene.h>
#include <SelectedComponent.h>

using namespace ursine;
using namespace ecs;

ENTITY_SYSTEM_DEFINITION(ShieldFXSystem);

ShieldFXSystem::ShieldFXSystem(World *world)
    : FilterSystem(world, Filter( ).All<ShieldFX>( ))
{
#if defined(URSINE_WITH_EDITOR)

    m_world->Listener(this)
        .On(WORLD_EDITOR_UPDATE, &FilterSystem::onUpdate);

#endif
}

void ShieldFXSystem::Process(const ursine::ecs::EntityHandle & entity)
{
    auto playstate = entity->GetWorld( )->GetOwner( )->GetPlayState( );

#if defined(URSINE_WITH_EDITOR)

    if (playstate != PS_PLAYING && !entity->HasComponent<Selected>( ))
        return;

#else

    if (playstate != PS_PLAYING)
        return;

#endif

    auto fragmentComponent = entity->GetComponent<ModelFragmenter>( );
    auto shieldComponent = entity->GetComponent<ShieldFX>();
    
    auto shieldState = shieldComponent->GetShieldState( );

    float dt = Application::Instance->GetDeltaTime( );

    fragmentComponent->SetTextureUV( 
        fragmentComponent->GetTextureUV( ) + shieldComponent->GetTextureVelocity( ) * dt
    );
}