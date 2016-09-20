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
#if defined(URSINE_WITH_EDITOR)

    if (entity->GetWorld( )->GetOwner( )->IsPaused( ) && !entity->HasComponent<Selected>( ))
        return;

#endif

    auto *model = entity->GetComponent<Model3D>( );
    auto *textureAnimator = entity->GetComponent<TextureAnimator>( );

    float dt = Application::Instance->GetDeltaTime( );

    if(textureAnimator->GetIsDiffusePlaying( ))
        model->SetTextureUVOffset( model->GetTextureUVOffset( ) + textureAnimator->GetDiffuseTextureVelocity( ) * dt );

    if (textureAnimator->GetIsEmissivePlaying( ))
        model->SetEmissiveTextureUVOffset(model->GetEmissiveTextureUVOffset( ) + textureAnimator->GetEmissiveTextureVelocity( ) * dt);
}