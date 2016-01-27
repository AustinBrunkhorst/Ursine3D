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

#include "GFXAPIDefines.h"
#include <Core/Graphics/API/GfxAPI.h>
#include "RenderableComponentBase.h"
#include "Filter.h"
#include <Game Engine/Scene/Component/Native Components/AnimatorComponent.h>
#include <Game Engine/Scene/Component/Native Components/FbxSceneRootNodeComponent.h>


namespace ursine
{
    class GfxAPI;

    namespace ecs
    {
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

            void SortCameraArray(void);

        private:
            graphics::GfxAPI *m_graphics;

            void OnInitialize(void) override;
            void OnRemove(void) override;

            // vector of cameras sorted based on their render layer (low to high)
            std::vector< Component::Handle<Camera> > m_cameras;

            static bool cameraSortPredicate(Component::Handle<Camera> a, Component::Handle<Camera> b);


            typedef std::vector<RenderableComponentBase*> RenderableVector;
            typedef std::unordered_map<EntityUniqueID, RenderableVector> RenderableMap;
            RenderableMap m_renderableMap;

            std::vector< Component::Handle<Animator> > m_animators;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onRender(EVENT_HANDLER(World));

            void addRenderable(Entity *entity, RenderableComponentBase* renderable);
            void removeRenderable(Entity *entity, RenderableComponentBase* renderable);
        } Meta(Enable);
    }
}