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

#include <Application.h>
#include <Scene.h>
#include <SelectedComponent.h>
#include <Model3DComponent.h>

#include "TextureAnimatorSystem.h"
#include "TextureAnimatorComponent.h"

using namespace ursine;
using namespace ecs;

ENTITY_SYSTEM_DEFINITION(TextureAnimatorSystem);

TextureAnimatorSystem::TextureAnimatorSystem(World *world)
    : FilterSystem(world, Filter( ).All<TextureAnimator>( ))
{
#if defined(URSINE_WITH_EDITOR)

    m_world->Listener(this)
        .On(WORLD_EDITOR_UPDATE, &FilterSystem::onUpdate);

#endif
}

void TextureAnimatorSystem::Process(const ursine::ecs::EntityHandle & entity)
{
    auto playstate = entity->GetWorld( )->GetOwner( )->GetPlayState( );

#if defined(URSINE_WITH_EDITOR)

    if (playstate != PS_PLAYING && !entity->HasComponent<Selected>( ))
        return;

#else

    if (playstate != PS_PLAYING)
        return;

#endif

    auto *model = entity->GetComponent<Model3D>( );
    auto *textureAnimator = entity->GetComponent<TextureAnimator>( );

    float dt = Application::Instance->GetDeltaTime( );

    if(textureAnimator->GetIsPlaying( ))
        model->SetTextureUVOffset( model->GetTextureUVOffset( ) + textureAnimator->GetTextureVelocity( ) * dt );
}