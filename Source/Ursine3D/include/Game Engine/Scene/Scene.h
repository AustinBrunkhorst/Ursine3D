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

#include "GameContext.h"

#include "World.h"

#include "UIScreenManager.h"
#include "ResourceManager.h"

#include "GFXAPIDefines.h"

#include "ScenePlayState.h"
#include "SceneEvent.h"

#include "DeltaTime.h"

namespace ursine
{
    class Scene : public EventDispatcher<SceneEventType>
    {
    public:
        Scene(void);
        ~Scene(void);

        GameContext *GetGameContext(void);
        void SetGameContext(GameContext *context);

        ecs::World *GetActiveWorld(void) const;

        void SetActiveWorld(ecs::World::Handle world);
        bool SetActiveWorld(const resources::ResourceReference &reference);

        graphics::GfxHND GetViewport(void) const;
        void SetViewport(graphics::GfxHND viewport);

        ScenePlayState GetPlayState(void) const;
        void SetPlayState(ScenePlayState state);

        UIScreenManager &GetScreenManager(void);
        resources::ResourceManager &GetResourceManager(void);

        void Step(void) const;

        void Update(DeltaTime dt) const;
        void Render(void) const;

        void LoadConfiguredSystems(void);
    private:
        Scene(const Scene &rhs) = delete;
        Scene &operator=(const Scene &rhs) = delete;

        GameContext *m_gameContext;

        ScenePlayState m_playState;

        graphics::GfxHND m_viewport;

        ecs::World::Handle m_activeWorld;

        UIScreenManager m_screenManager;
        resources::ResourceManager m_resourceManager;
    };
}