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

        void Update(DeltaTime dt);
        void Render(void);

    private:
        graphics::GfxHND m_viewport;

        ecs::World m_world;
    };
}