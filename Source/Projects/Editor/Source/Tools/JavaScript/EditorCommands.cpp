#include "Precompiled.h"

#include "EditorCommands.h"

#include "Editor.h"
#include "EditorToolSystem.h"

#include <UIManager.h>

#include <SelectedComponent.h>
#include <CameraComponent.h>
#include <BoxColliderComponent.h>
#include <CylinderColliderComponent.h>
#include <SphereColliderComponent.h>
#include <LightComponent.h>
#include <SpriteTextComponent.h>
#include <ParticleSystemComponent.h>
#include <ParticleEmitterComponent.h>
#include <ParticleAnimatorComponent.h>
#include <ParticleColorAnimatorComponent.h>

using namespace ursine;
using namespace std::placeholders;

namespace editor_commands
{
    namespace
    {
        Scene &getScene(void);
        ecs::World *getActiveWorld(void);
        ecs::EntityHandle createEntity(const std::string &name = "Empty Entity");
    }

    JSFunction(GetEditorCommands)
    {
        Json::array commands;

        static const auto prefixLength = strlen( "editor_commands::" );

        for (auto &function : meta::Type::GetGlobalFunctions( ))
        {
            auto *enabled = function.GetMeta( ).GetProperty<EditorCommand>( );

            if (!enabled)
                continue;

            commands.emplace_back( function.GetName( ).substr( prefixLength ) );
        }

        CefRefPtr<CefV8Value> items;

        JsonSerializer::Deserialize( commands, items );

        return items;
    }

    ///////////////////////////////////////////////////////////////////////////

    JSFunction(CreateEmptyEntity)
    {
        createEntity( );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreateCamera)
    {
        Application::PostMainThread( [] 
        {
            createEntity( "Camera" )
                ->AddComponent<ecs::Camera>( )
                ->SetViewportSize( { 1, 1 } );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreatePlane)
    {
        Application::PostMainThread( [] 
        {
            createEntity( "Plane" )
                ->AddComponent<ecs::BoxCollider>( )
                ->SetDimensions( { 5, 1, 5 } );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreateBox)
    {
        Application::PostMainThread( [] 
        {
            createEntity( "Box" )
                ->AddComponent<ecs::BoxCollider>( );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreateCylinder)
    {
        Application::PostMainThread( [] 
        {
            createEntity( "Cylinder" )
                ->AddComponent<ecs::CylinderCollider>( );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreateSphere)
    {
        Application::PostMainThread( [] 
        {
            createEntity( "Sphere" )
                ->AddComponent<ecs::SphereCollider>( );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreatePointLight)
    {
        Application::PostMainThread( [] 
        {
            createEntity( "Point Light" )
                ->AddComponent<ecs::Light>( )
                ->SetLightType( ecs::LightType::Point );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreateSpotLight)
    {
        Application::PostMainThread( [] 
        {
            createEntity( "Spot Light" )
                ->AddComponent<ecs::Light>( )
                ->SetLightType( ecs::LightType::Spot );
        } );
        
        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreateDirectionalLight)
    {
        Application::PostMainThread( [] 
        {
            createEntity( "Directional Light" )
                ->AddComponent<ecs::Light>( )
                ->SetLightType( ecs::LightType::Directional );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreateParticleSystem)
    {
        Application::PostMainThread( [] 
        {
            auto entity = createEntity( "Particle System" );

            entity->AddComponent<ecs::ParticleSystem>( );
            entity->AddComponent<ecs::ParticleEmitter>( );
            entity->AddComponent<ecs::ParticleAnimator>( );
            entity->AddComponent<ecs::ParticleColorAnimator>( );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(CreateSpriteText)
    {
        Application::PostMainThread( [] 
        {
            auto entity = createEntity( "Sprite Text" );

            entity->AddComponent<ecs::SpriteText>( );
        } );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(InspectEditorUI)
    {
        std::string debugURL( "http://localhost:" );

        debugURL += std::to_string( kUIDebuggingPort );

        utils::OpenPath( debugURL );

        return CefV8Value::CreateUndefined( );
    }

    JSFunction(ReloadEditorUI)
    {
        if (getScene( ).GetPlayState( ) != PS_EDITOR)
        {
            NotificationConfig error;

            error.type = NOTIFY_ERROR;
            error.header = "Error";
            error.message = "Unable to reload UI with an active game session.";

            EditorPostNotification( error );

            return CefV8Value::CreateBool( false );
        }

        CefRefPtr<CefV8Value> returnValue;
        CefRefPtr<CefV8Exception> e;

        CefV8Context::GetCurrentContext( )->Eval( "location.reload(true)", returnValue, e );

        return returnValue;
    }

    namespace
    {
        Scene &getScene(void)
        {
            return GetCoreSystem( Editor )->GetProject( )->GetScene( );
        }

        ecs::World *getActiveWorld(void)
        {
            return getScene( ).GetActiveWorld( );
        }

        ecs::EntityHandle createEntity(const std::string &name)
        {
            auto *world = getActiveWorld( );

            auto entity = world->CreateEntity( name );

            auto *tools = world->GetEntitySystem<EditorToolSystem>( );

            if (tools)
                tools->ClearSelectedEntities( );

            auto *cameraSystem = world->GetEntitySystem<EditorCameraSystem>( );

            // move it to focus
            if (cameraSystem)
                entity->GetTransform( )->SetWorldPosition( cameraSystem->GetEditorFocusPosition( ) );

            entity->AddComponent<ecs::Selected>( );

            return entity;
        }
    }
}