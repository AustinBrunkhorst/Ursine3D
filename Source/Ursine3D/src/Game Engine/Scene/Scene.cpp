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

namespace ursine
{
    Scene::Scene(void)
        : m_playState( PS_EDITOR )
        , m_viewport( 0 )
        , m_world( std::make_shared<ecs::World>( ) )
    {
        
    }

    Scene::~Scene(void)
    {

    }

    ecs::World *Scene::GetWorld(void)
    {
        return m_world.get( );
    }

    void Scene::SetWorld(ecs::World *world)
    {
        m_world = ecs::World::Handle( world );
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
        m_playState = state;
    }

    void Scene::Step(void) const
    {
        m_world->Update( );
    }

    void Scene::Update(DeltaTime dt) const
    {
        switch (m_playState)
        {
        case PS_PAUSED:
        case PS_EDITOR:
            m_world->EditorUpdate( );
            break;
        case PS_PLAYING:
            m_world->Update( );
            break;
        }
    }

    void Scene::Render(void) const
    {
        switch (m_playState)
        {
        case PS_PAUSED:
        case PS_EDITOR:
            m_world->EditorRender( );
            break;
        case PS_PLAYING:
            m_world->Render( );
            break;
        }
    }

    void Scene::LoadConfiguredSystems(void)
    {
        auto *config = m_world->GetSettings( )->GetComponent<ecs::WorldConfig>( );

        auto *systemManager = m_world->GetSystemManager( );

        for (auto &system : config->GetSystems( ))
        {
			// If this system is the equivalent of null
			if (system.type.size( ) == 0)
				continue;

            auto type = meta::Type::GetFromName( system.type );

            if (!type.IsValid( ))
            {
                NotificationConfig error;

                error.type = NOTIFY_ERROR;
                error.header = "Play System";
                error.message = "Unknown world play system configured.";

                EditorPostNotification( error );
            }

            if (systemManager->HasSystem( type ))
            {
                NotificationConfig error;

                error.type = NOTIFY_ERROR;
                error.header = "Play System Exists";
                error.message = "World play system <strong class=\"highlight\">" + type.GetName( ) + "</strong> already exists.";

                EditorPostNotification( error );
            }

            systemManager->AddSystem( type );
        }
    }
}