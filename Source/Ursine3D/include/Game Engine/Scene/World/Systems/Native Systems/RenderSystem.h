/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "EntitySystem.h"

#include "GfxDefines.h"

namespace ursine
{
    class RenderableComponentBase;

    namespace graphics
    {
    class GfxAPI;
    }

    namespace ecs
    {
        class WorldConfig;
        class Camera;
        class Animator;

        enum RenderSystemEventType
        {
            RENDER_HOOK,
            RENDER_HOOK_EDITOR
        };

        struct RenderHookArgs : EventArgs
        {
            graphics::GfxHND camera;

            RenderHookArgs(graphics::GfxHND camera)
                : camera( camera ) { }
        };

        class RenderSystem 
            : public EntitySystem
            , public EventDispatcher<RenderSystemEventType>
        {
            ENTITY_SYSTEM;

        public:
            Meta(Enable)
            RenderSystem(World *world);
            ~RenderSystem(void);

            void SortCameraArray(void);

        private:
            typedef std::vector<RenderableComponentBase *> RenderableVector;
            typedef std::unordered_map<EntityID, RenderableVector> RenderableMap;

            graphics::GfxAPI *m_graphics;
            Component::Handle<WorldConfig> m_worldConfig;

            void OnInitialize(void) override;
            void OnRemove(void) override;
            void OnSceneReady(Scene *scene) override;

            // vector of cameras sorted based on their render layer (low to high)
            std::vector<Component::Handle<Camera>> m_cameras;

            static bool cameraSortPredicate(Component::Handle<Camera> a, Component::Handle<Camera> b);

            RenderableMap m_renderableMap;

            std::vector<Component::Handle<Animator>> m_animators;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));
            void onRender(EVENT_HANDLER(World));

        #if defined(URSINE_WITH_EDITOR)

            void onEditorUpdate(EVENT_HANDLER(World));
            void onEditorRender(EVENT_HANDLER(World));

        #endif

            void renderObjects(void);
            void renderCamera(Component::Handle<Camera> camera, RenderHookArgs &args, RenderSystemEventType hook);

            void addRenderable(const EntityHandle &entity, RenderableComponentBase *renderable);
            void removeRenderable(const EntityHandle &entity, RenderableComponentBase *renderable);
        } Meta(Enable, WhiteListMethods, AutoAddEntitySystem);
    }
}