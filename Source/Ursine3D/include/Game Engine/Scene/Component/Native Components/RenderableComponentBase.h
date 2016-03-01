/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableComponentBase.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "GFXAPIDefines.h"
#include "EventDispatcher.h"

namespace ursine
{
    namespace ecs
    {
        class RenderSystem;
    }

    class RenderableComponentBase
    {
    public:
        // lets us know if we need to update the matrix in the renderer
        bool dirty;

        RenderableComponentBase(std::function<void(void)> updateCallback);

        void OnInitialize(const ecs::EntityHandle &owner);
        void OnRemove(const ecs::EntityHandle &owner);

		graphics::GfxHND &GetHandle(void);
		void SetHandle(const graphics::GfxHND &handle);

    protected:
        friend class ecs::RenderSystem;

        graphics::GfxHND m_handle;

        void onTransformChange(EVENT_HANDLER(ecs::Entity));

        // the base class is responsible for implementing this function
        // and setting the appropriate values in the renderer
        std::function<void(void)> m_updateRenderer;
    };
}
