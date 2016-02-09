/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

    enum ScenePlayState
    {
        PS_EDITOR,
        PS_PLAYING,
        PS_PAUSED
    };

    class Scene
    {
    public:
        typedef std::shared_ptr<Scene> Handle;

        Scene(void);
        ~Scene(void);

        ecs::World *GetWorld(void);
        void SetWorld(ecs::World *world);

        graphics::GfxHND GetViewport(void) const;
        void SetViewport(graphics::GfxHND viewport);

        ScenePlayState GetPlayState(void) const;
        void SetPlayState(ScenePlayState state);

        void Step(void) const;

        void Update(DeltaTime dt) const;
        void Render(void) const;

        void LoadConfiguredSystems(void);

    private:
        ScenePlayState m_playState;

        graphics::GfxHND m_viewport;

        ecs::World::Handle m_world;
    };
}