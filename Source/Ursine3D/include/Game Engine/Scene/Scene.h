#pragma once

#include "World.h"
#include "GFXAPIDefines.h"
#include <Core/Graphics/API/GfxAPI.h>

namespace ursine
{
    class GfxAPI;

    class Scene
    {
    public:
        Scene(void);

        ecs::World &GetWorld(void);

        graphics::GfxHND GetViewport(void) const;
        void SetViewport(graphics::GfxHND viewport);

        graphics::GfxHND GetEditorCamera(void) const;
        void SetEditorCamera(graphics::GfxHND camera);

        void Update(DeltaTime dt);
        void Render(void);

    private:
        graphics::GfxAPI *m_graphics;
        graphics::GfxHND m_viewport;
        graphics::GfxHND m_editorCamera;

        ecs::World m_world;
    };
}