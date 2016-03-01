#include "Precompiled.h"

#include "EditorCommands.h"

#include "Editor.h"
#include "EditorToolSystem.h"

#include <UIManager.h>

#include <FileSystem.h>
#include <FileDialog.h>
#include <EntitySerializer.h>

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
        void doLoadArchetype(ecs::World *world, int selectedFilter, const fs::FileList &files);

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

    JSFunction(CreateEntityFromArchetype)
    {
        auto *editor = GetCoreSystem( Editor );
        auto *world = editor->GetProject( )->GetScene( ).GetActiveWorld( );

        fs::FileDialog openDialog;

        openDialog.config.mode = fs::FDM_OPEN;
        openDialog.config.initialPath = "";
        openDialog.config.windowTitle = "Create Entity From Archetype";
        openDialog.config.parentWindow = editor->GetMainWindow( ).GetWindow( );
        openDialog.config.filters = {
            { "Archetype Files", { "*.uatype" } }
        };

        auto result = openDialog.Open( );

        doLoadArchetype( world, result.selectedFilterIndex, result.selectedFiles );

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

    JSFunction(InspectEditorUI)
    {
        std::string debugURL( "http://localhost:" );

        debugURL += std::to_string( UIManager::REMOTE_DEBUGGING_PORT );

        utils::OpenPath( debugURL );

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

    namespace
    {
        void doLoadArchetype(
            ecs::World *world,
            int selectedFilter,
            const fs::FileList &files
        )
        {
            if (files.empty( ))
                return;

            auto &file = files.front( );
            auto filename = file.stem( ).string( );

            Application::PostMainThread( [=] 
            {
                try
                {
                    std::string jsonData;

                    if (!fs::LoadAllText( file.string( ), jsonData ))
                    {
                        throw ecs::SerializationException( "Unable to load file." );
                    }
       
                    std::string jsonError;

                    auto data = Json::parse( jsonData, jsonError );

                    if (!jsonError.empty( ))
                    {
                        throw ecs::SerializationException( jsonError );
                    }

                    auto entity = ecs::EntitySerializer( ).DeserializeArchetype( 
                        world, 
                        data 
                    );

                    entity->SetName( filename + " Archetype" );

                    entity->AddComponent<ecs::Selected>( );
                }
                catch (ecs::SerializationException &e)
                {
                    UWarning(
                        "Unable to load archetype.\nerror: %s\nfile: %s",
                        e.GetError( ).c_str( ),
                        file.string( ).c_str( )
                    );

                    auto *editor = GetCoreSystem( Editor );

                    NotificationConfig error;

                    error.type = NOTIFY_ERROR;
                    error.header = "Load Error";
                    error.message = "Unable to load archetype.";

                    error.buttons =
                    {
                        { "Open Error Log", 
                            [](Notification &notification)
                            {
                                notification.Close( );

                                utils::OpenPath( URSINE_ERROR_LOG_FILE );
                            } 
                        }
                    };

                    editor->PostNotification( error );
                }
            } );
        }

        ecs::World *getActiveWorld(void)
        {
            return GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetActiveWorld( );
        }

        ecs::EntityHandle createEntity(const std::string &name)
        {
            auto *world = getActiveWorld( );

            auto entity = world->CreateEntity( name );

            auto *tools = world->GetEntitySystem<EditorToolSystem>( );

            if (tools)
                tools->ClearSelectedEntities( );

            entity->AddComponent<ecs::Selected>( );

            return entity;
        }
    }
}