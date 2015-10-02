#include "UrsinePrecompiled.h"

#include "Scene.h"

#include "GfxAPI.h"

namespace ursine
{
    Scene::Scene(void)
        : m_graphics( GetCoreSystem( GfxAPI ) )
        , m_viewport( 0 )
    {
        
    }

    ecs::World &Scene::GetWorld(void)
    {
        return m_world;
    }

    GFXHND Scene::GetViewport(void) const
    {
        return m_viewport;
    }

    void Scene::SetViewport(GFXHND viewport)
    {
        m_viewport = viewport;
    }

    GFXHND Scene::GetEditorCamera(void) const
    {
        return m_editorCamera;
    }

    void Scene::SetEditorCamera(GFXHND camera)
    {
        m_editorCamera = camera;
    }

    void Scene::Update(DeltaTime dt)
    {
        m_world.Update( );
    }

    void Scene::Render(void)
    {
        m_world.Render( );
    }
}