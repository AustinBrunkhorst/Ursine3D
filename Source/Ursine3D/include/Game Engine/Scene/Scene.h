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
#include "ResourceManager.h"
#include "GFXAPIDefines.h"

#include "SceneEvent.h"

namespace ursine
{
    class GfxAPI;

    enum ScenePlayState
    {
        PS_EDITOR,
        PS_PLAYING,
        PS_PAUSED
    };

    class Scene : public EventDispatcher<SceneEventType>
    {
    public:
        Scene(void);
        ~Scene(void);

        ecs::World *GetActiveWorld(void);

        void SetActiveWorld(ecs::World::Handle world);
        bool SetActiveWorld(const resources::ResourceReference &reference);

        graphics::GfxHND GetViewport(void) const;
        void SetViewport(graphics::GfxHND viewport);

        ScenePlayState GetPlayState(void) const;
        void SetPlayState(ScenePlayState state);

        resources::ResourceManager &GetResourceManager(void);

        void Step(void) const;

        void Update(DeltaTime dt) const;
        void Render(void) const;

        void LoadConfiguredSystems(void);

    private:
        Scene(const Scene &rhs) = delete;
        Scene &operator=(const Scene &rhs) = delete;

        ScenePlayState m_playState;

        graphics::GfxHND m_viewport;

        ecs::World::Handle m_activeWorld;

        resources::ResourceManager m_resourceManager;
    };
}