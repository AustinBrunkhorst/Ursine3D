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
        typedef std::shared_ptr<Scene> Handle;

        Scene(void);

        ecs::World::Handle GetWorld(void);
        void SetWorld(ecs::World::Handle world);

        graphics::GfxHND GetViewport(void) const;
        void SetViewport(graphics::GfxHND viewport);

        void Update(DeltaTime dt);
        void Render(void);

    private:
        graphics::GfxHND m_viewport;

        ecs::World::Handle m_world;
    };
}