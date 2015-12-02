#pragma once

#include "EntitySystem.h"

#include "GFXAPIDefines.h"
#include <Core/Graphics/API/GfxAPI.h>
#include <Game Engine/Scene/Component/Native Components/AnimatorComponent.h>

namespace ursine
{
    class GfxAPI;

    namespace ecs
    {
        class Renderable;
        class Camera;

        enum RenderSystemEventType
        {
            RENDER_HOOK
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
            RenderSystem(World *world);
            ~RenderSystem(void);

        private:
            graphics::GfxAPI *m_graphics;

            void OnInitialize(void) override;
            void OnRemove(void) override;

            std::unordered_map<EntityUniqueID, ursine::ecs::Camera*> m_cameras;
            std::unordered_map<EntityUniqueID, Renderable*> m_renderable;
            std::unordered_map<EntityUniqueID, Animator*> m_animator;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onRender(EVENT_HANDLER(World));
        } Meta(Enable);
    }
}