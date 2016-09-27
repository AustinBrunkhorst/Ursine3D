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

#include "GFXDefines.h"

#include "SceneEvent.h"

#include "DeltaTime.h"

namespace ursine
{
    class Scene : public EventDispatcher<uint32>
    {
    public:
        Scene(void);
        virtual ~Scene(void);

        GameContext *GetGameContext(void);
        void SetGameContext(GameContext *context);

        ecs::World *GetActiveWorld(void) const;

        void SetActiveWorld(ecs::World::Handle world);
        bool SetActiveWorld(const resources::ResourceReference &reference, bool ignoreCache = true);

        graphics::GfxHND GetViewport(void) const;
        void SetViewport(graphics::GfxHND viewport);

        virtual void SetPaused(bool paused);
        virtual bool IsPaused(void) const;

        UIScreenManager &GetScreenManager(void);
        resources::ResourceManager &GetResourceManager(void);

        virtual void Update(DeltaTime dt) const;
        virtual void Render(void) const;

        void LoadConfiguredSystems(void);

    protected:
        Scene(const Scene &rhs) = delete;
        Scene &operator=(const Scene &rhs) = delete;

        bool m_paused;

        GameContext *m_gameContext;

        graphics::GfxHND m_viewport;

        ecs::World::Handle m_activeWorld;

        UIScreenManager m_screenManager;
        resources::ResourceManager m_resourceManager;
    };
}