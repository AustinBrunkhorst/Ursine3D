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
        , m_activeWorld( nullptr )
    {
        
    }

    Scene::~Scene(void)
    {

    }

    ecs::World *Scene::GetActiveWorld(void)
    {
        return m_activeWorld.get( );
    }

    void Scene::SetActiveWorld(ecs::World *world)
    {
        m_activeWorld = ecs::World::Handle( world );
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
        if (m_activeWorld)
            m_activeWorld->Update( );
    }

    void Scene::Update(DeltaTime dt) const
    {
        if (!m_activeWorld)
            return;

        switch (m_playState)
        {
        case PS_PAUSED:
        case PS_EDITOR:
            m_activeWorld->EditorUpdate( );
            break;
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
        case PS_PAUSED:
        case PS_EDITOR:
            m_activeWorld->EditorRender( );
            break;
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

            if (!type.IsValid( ))
            {
                NotificationConfig error;

                error.type = NOTIFY_ERROR;
                error.header = "Play System";
                error.message = "Unknown world play system configured.";

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

            systemManager->AddSystem( type );
        }
    }
}