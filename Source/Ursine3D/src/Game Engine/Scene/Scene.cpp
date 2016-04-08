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

#if defined(URSINE_WITH_EDITOR)

#define SCENE_STARTING_STATE PS_EDITOR

#else

#define SCENE_STARTING_STATE PS_PLAYING

#endif

namespace ursine
{
    Scene::Scene(void)
        : EventDispatcher( this )
        , m_gameContext( nullptr )
        , m_playState( SCENE_STARTING_STATE )
        , m_viewport( 0 )
        , m_activeWorld( nullptr ) { }

    Scene::~Scene(void) { }

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

    ScenePlayState Scene::GetPlayState(void) const
    {
        return m_playState;
    }

    void Scene::SetPlayState(ScenePlayState state)
    {
        if (state == m_playState)
            return;

        ScenePlayStateChangedArgs e( m_playState, state );

        m_playState = state;

        Dispatch( SCENE_PLAYSTATE_CHANGED, &e );
    }

    UIScreenManager &Scene::GetScreenManager(void)
    {
        return m_screenManager;
    }

    resources::ResourceManager &Scene::GetResourceManager(void)
    {
        return m_resourceManager;
    }

    void Scene::Step(void) const
    {
        if (m_activeWorld)
            m_activeWorld->Update( );

        SceneFrameSteppedArgs e( Application::Instance->GetDeltaTime( ) );

        Dispatch( SCENE_FRAME_STEPPED, &e );
    }

    void Scene::Update(DeltaTime dt) const
    {
        if (!m_activeWorld)
            return;

        switch (m_playState)
        {
    #if defined(URSINE_WITH_EDITOR)

        case PS_PAUSED:
        case PS_EDITOR:
            m_activeWorld->EditorUpdate( );
            break;

    #endif
        case PS_PLAYING:
            m_activeWorld->Update( );
            break;
        }
    }

    void Scene::Render(void) const
    {
        if (!m_activeWorld)
            return;

        switch (m_playState)
        {
    #if defined(URSINE_WITH_EDITOR)

        case PS_PAUSED:
        case PS_EDITOR:
            m_activeWorld->EditorRender( );
            break;

    #endif
        case PS_PLAYING:
            m_activeWorld->Render( );
            break;
        }
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