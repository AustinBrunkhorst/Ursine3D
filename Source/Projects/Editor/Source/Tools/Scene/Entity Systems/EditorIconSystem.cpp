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

#include "Editor.h"

#include "EditorIconSystem.h"
#include "EditorIconComponent.h"

#include <LightComponent.h>
#include <CameraComponent.h>
#include <ParticleSystemComponent.h>

#include <EditorToolResources.h>
#include <TextureData.h>

ENTITY_SYSTEM_DEFINITION( EditorIconSystem );

using namespace ursine;

EditorIconSystem::EditorIconSystem(ecs::World *world)
    : EntitySystem( world )
    , m_toolResources( GetCoreSystem( Editor )->GetProject( )->GetBuiltInResourceManager( ) )
{
    
}

void EditorIconSystem::OnSceneReady(Scene *scene)
{
    m_world->Listener( this )
        .On( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd )
        .On( ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, &EditorIconSystem::onLightTypeChange );

    auto entities = m_world->GetActiveEntities( );

    ecs::ComponentEventArgs args( 
        ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, 
        nullptr, 
        nullptr
    );

    for (auto &entity : entities)
    {
        auto components = utils::MakeArray<ecs::Component*>(
            entity->GetComponent<ecs::Camera>( ),
            entity->GetComponent<ecs::Light>( ),
            entity->GetComponent<ecs::ParticleSystem>( )
        );

        args.entity = entity;

        for (auto *component : components)
        {
            if (!component)
                continue;

            args.component = component;

            onIconAdd( m_world, &args );
        }
    }
}

void EditorIconSystem::OnRemove(void)
{
    m_world->Listener( this )
        .Off( ecs::WORLD_ENTITY_COMPONENT_ADDED, &EditorIconSystem::onIconAdd )
        .Off( ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, &EditorIconSystem::onLightTypeChange );
}

void EditorIconSystem::onIconAdd(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    auto comp = args->component;
    auto entity = args->entity;

    auto *editorIcon = entity->GetComponent<EditorIcon>( );

    if (comp->Is<ecs::Camera>( ))
    {
        if (!editorIcon)
            editorIcon = entity->AddComponent<EditorIcon>( );

        resources::ResourceReference iconRef = editor_resources::IconCamera;

        auto *texture = iconRef.Load<resources::TextureData>( m_toolResources );

        UAssert( texture != nullptr,
            "Unable to load editor camera icon."
        );

        editorIcon->SetIcon( texture->GetTextureHandle( ) );
    }
    else if (comp->Is<ecs::ParticleSystem>( ))
    {
        if (!editorIcon)
            editorIcon = entity->AddComponent<EditorIcon>( );

        resources::ResourceReference iconRef = editor_resources::IconParticleSystem;

        auto *texture = iconRef.Load<resources::TextureData>( m_toolResources );

        UAssert( texture != nullptr,
            "Unable to load editor particle system icon."
        );

        editorIcon->SetIcon( texture->GetTextureHandle( ) );
    }
    else if (comp->Is<ecs::Light>( ))
    {
        if (!editorIcon)
            editorIcon = entity->AddComponent<EditorIcon>( );

        auto *light = entity->GetComponent<ecs::Light>( );

        setLightIcon( light->GetLightType( ), editorIcon );
    }
}

void EditorIconSystem::onLightTypeChange(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::EditorComponentChangedArgs);

    if (args->component->Is<ecs::Light>( ) && args->field == "type")
    {
        auto type = args->entity->GetComponent<ecs::Light>( )->GetLightType( );
        auto icon = args->entity->GetComponent<EditorIcon>( );

        if (!icon)
            return;

        setLightIcon( type, icon );
    }
}

void EditorIconSystem::setLightIcon(ecs::LightType type, EditorIcon *icon)
{
    resources::ResourceReference iconResource;

    switch (type)
    {
    case ecs::LightType::Directional:
        iconResource = editor_resources::IconDirectionalLight;
        break;

    case ecs::LightType::Point:
        iconResource = editor_resources::IconPointLight;
        break;

    case ecs::LightType::Spot:
        iconResource = editor_resources::IconSpotLight;
        break;
    }

    auto *texture = iconResource.Load<resources::TextureData>( m_toolResources );

    UAssert( texture != nullptr,
        "Unable to load editor icon for light type '%s'.",
        typeof( ecs::LightType ).GetEnum( ).GetKey( type ).c_str( )
    );

    icon->SetIcon( texture->GetTextureHandle( ) );
}
