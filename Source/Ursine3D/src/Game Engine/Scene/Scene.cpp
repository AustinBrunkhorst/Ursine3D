#include "UrsinePrecompiled.h"

#include "Scene.h"

#include "GfxAPI.h"

namespace ursine
{
    Scene::Scene(void)
        : m_viewport( 0 )
        , m_world( std::make_shared<ecs::World>( ) )
    {
        
    }

    ecs::World::Handle Scene::GetWorld(void)
    {
        return m_world;
    }

    graphics::GfxHND Scene::GetViewport(void) const
    {
        return m_viewport;
    }

    void Scene::SetViewport(graphics::GfxHND viewport)
    {
        m_viewport = viewport;
    }

    void Scene::Update(DeltaTime dt)
    {
        m_world->Update( );
    }

    void Scene::Render(void)
    {
        m_world->Render( );
    }
}