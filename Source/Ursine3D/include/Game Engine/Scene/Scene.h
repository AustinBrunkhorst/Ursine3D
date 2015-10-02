#pragma once

#include "World.h"
#include "GFXAPIDefines.h"

namespace ursine
{
    class GfxAPI;

    class Scene
    {
    public:
        Scene(void);

        ecs::World &GetWorld(void);

        GFXHND GetViewport(void) const;
        void SetViewport(GFXHND viewport);

        GFXHND GetEditorCamera(void) const;
        void SetEditorCamera(GFXHND camera);

        void Update(DeltaTime dt);
        void Render(void);

    private:
        GfxAPI *m_graphics;
        GFXHND m_viewport;
        GFXHND m_editorCamera;

        ecs::World m_world;
    };
}