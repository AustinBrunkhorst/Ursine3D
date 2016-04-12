/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Scene.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Scene.h"

#include "WorldConfigComponent.h"
#include "SystemManager.h"

#include "Notification.h"

#include "WorldData.h"

namespace ursine
{
    Scene::Scene(void)
        : EventDispatcher( this )
        , m_paused( false )
        , m_gameContext( nullptr )
        , m_viewport( 0 )
        , m_activeWorld( nullptr ) { }

    Scene::~Scene(void)
    {
        m_activeWorld = nullptr;
    }

    GameContext *Scene::GetGameContext(void)
    {
        return m_gameContext;
    }

    void Scene::SetGameContext(GameContext *context)
    {
        m_gameContext = context;
    }

    ecs::World *Scene::GetActiveWorld(void) const
    {
        return m_activeWorld.get( );
    }

    void Scene::SetActiveWorld(ecs::World::Handle world)
    {
        SceneWorldChangedArgs e( m_activeWorld, nullptr );

        m_activeWorld = world;

        m_activeWorld->setOwner( this );

        Dispatch( SCENE_WORLD_CHANGED, &e );
    }

    bool Scene::SetActiveWorld(const resources::ResourceReference &reference, bool ignoreCache /*= true*/)
    {
        auto *worldData = reference.Load<resources::WorldData>( m_resourceManager, ignoreCache );

        if (!worldData)
            return false;

        SceneWorldChangedArgs e( m_activeWorld, &reference );

        m_activeWorld = worldData->GetData( );

        UAssert( m_activeWorld,
            "Resource world data was null."    
        );

        m_activeWorld->setOwner( this );

        Dispatch( SCENE_WORLD_CHANGED, &e );

        // make sure it doesn't stay in the cache
        if (ignoreCache)
            m_resourceManager.UnloadReference( reference );

        return true;
    }

    graphics::GfxHND Scene::GetViewport(void) const
    {
        return m_viewport;
    }

    void Scene::SetViewport(graphics::GfxHND viewport)
    {
        m_viewport = viewport;
    }

    void Scene::SetPaused(bool paused)
    {
        m_paused = paused;
    }

    bool Scene::IsPaused(void) const
    {
        return m_paused;
    }

    UIScreenManager &Scene::GetScreenManager(void)
    {
        return m_screenManager;
    }

    resources::ResourceManager &Scene::GetResourceManager(void)
    {
        return m_resourceManager;
    }

    void Scene::Update(DeltaTime dt) const
    {
        if (m_activeWorld && !m_paused)
            m_activeWorld->Update( );
    }

    void Scene::Render(void) const
    {
        if (m_activeWorld)
            m_activeWorld->Render( );
    }

    void Scene::LoadConfiguredSystems(void)
    {
        auto *config = m_activeWorld->GetSettings( )->GetComponent<ecs::WorldConfig>( );

        auto *systemManager = m_activeWorld->GetSystemManager( );

        for (auto &system : config->GetSystems( ))
        {
			// If this system is the equivalent of null
			if (system.type.size( ) == 0)
				continue;

            auto type = meta::Type::GetFromName( system.type );

        #if defined(URSINE_WITH_EDITOR)

            if (!type.IsValid( ))
            {
                NotificationConfig error;

                error.type = NOTIFY_ERROR;
                error.header = "Play System";
                error.message = "Unknown world play system <strong class=\"highlight\">"+ system.type +"</strong> configured.";

                EditorPostNotification( error );

                continue;
            }

            if (systemManager->HasSystem( type ))
            {
                NotificationConfig error;

                error.type = NOTIFY_ERROR;
                error.header = "Play System Exists";
                error.message = "World play system <strong class=\"highlight\">" + type.GetName( ) + "</strong> already exists.";

                EditorPostNotification( error );

                continue;
            }

        #else

            UAssert( type.IsValid( ),
                "Unknown world play system '%s' configured.",
                system.type.c_str( )
            );

            UAssert( !systemManager->HasSystem( type ),
                "World play system '%s' already exists.",
                type.GetName( ).c_str( )
            );

        #endif

            systemManager->AddSystem( type );
        }
    }
}