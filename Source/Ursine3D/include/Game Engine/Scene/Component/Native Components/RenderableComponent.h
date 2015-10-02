#pragma once

#include "Component.h"

#include "GFXAPIDefines.h"

namespace ursine
{
    namespace ecs
    {
        class Renderable : public Component
        {
            NATIVE_COMPONENT;

        public:
            Renderable(void);

            Meta(Disable)
            graphics::GfxHND GetHandle(void) const;

            Meta(Disable)
            void SetHandle(graphics::GfxHND handle);

        private:
            friend class RenderSystem;

            graphics::GfxHND m_handle;
        } Meta(Enable, DisplayName( "Renderable" ));
    }
}