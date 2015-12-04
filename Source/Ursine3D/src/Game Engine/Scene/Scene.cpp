#include "UrsinePrecompiled.h"

#include "Scene.h"

#include "GfxAPI.h"

namespace ursine
{
    Scene::Scene(void)
        : m_paused( true )
        , m_viewport( 0 )
        , m_world( std::make_shared<ecs::World>( ) )
    {
        
    }

    ecs::World::Handle Scene::GetWorld(void)
    {
        return m_world;
    }

    void Scene::SetWorld(ecs::World::Handle world)
    {
        m_world = world;
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

    void Scene::Step(void) const
    {
        m_world->Update( );
    }

    void Scene::Update(DeltaTime dt) const
    {
        if (!m_paused)
            m_world->Update( );
    }

    void Scene::Render(void) const
    {
        m_world->Render( );
    }
}