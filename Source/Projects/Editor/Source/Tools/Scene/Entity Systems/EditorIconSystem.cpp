/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorIconSystem.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorIconSystem.h"
#include "EditorIconComponent.h"

#include <LightComponent.h>
#include <CameraComponent.h>
#include "GUID.h"
#include "Scene.h"
#include <Core/Resources/Types/TextureData.h>

ENTITY_SYSTEM_DEFINITION( EditorIconSystem );

using namespace ursine;

EditorIconSystem::EditorIconSystem(ecs::World* world)
    : EntitySystem( world )
{
    
}

void EditorIconSystem::OnInitialize(void)
{
    m_world->Listener( this )
        .On( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd );
}

void EditorIconSystem::OnRemove(void)
{
    m_world->Listener( this )
        .Off( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd );
}

void EditorIconSystem::onIconAdd(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    auto comp = args->component;

    // if the object added was a selected component
    if (comp->Is<ecs::Light>( ))
    {
        if ( !args->entity->HasComponent<EditorIcon>( ) )
            args->entity->AddComponent<EditorIcon>( );

        auto testReference = m_world->GetOwner()->GetResourceManager().CreateReference( GUIDStringGenerator()("11361b8e-1ca2-4754-a0d3-ab83f60adf22") );
        auto *texHandle = testReference.Load<resources::TextureData>( );

        args->entity->GetComponent<EditorIcon>()->SetIcon( texHandle->GetTextureHandle() );
    }
    else if (comp->Is<ecs::Camera>( ))
    {
        if ( !args->entity->HasComponent<EditorIcon>( ) )
            args->entity->AddComponent<EditorIcon>( );

        auto testReference = m_world->GetOwner()->GetResourceManager().CreateReference(GUIDStringGenerator()("0dca7489-2a8d-4fb0-80be-3e2f3ce5c235"));
        auto *texHandle = testReference.Load<resources::TextureData>();

        args->entity->GetComponent<EditorIcon>()->SetIcon(texHandle->GetTextureHandle());
        
    }
}