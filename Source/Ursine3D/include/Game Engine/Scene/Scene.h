/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Scene.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

        void SetPaused(bool paused);
        bool IsPaused(void) const;

        void Step(void) const;

        void Update(DeltaTime dt) const;
        void Render(void) const;

    private:
        bool m_paused;

        graphics::GfxHND m_viewport;

        ecs::World::Handle m_world;
    };
}