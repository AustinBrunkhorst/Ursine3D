#pragma once

#include "Component.h"

#include "Camera.h"

#include "GFXAPIDefines.h"

namespace ursine
{
    namespace ecs
    {
        class Camera : public Component
        {
            NATIVE_COMPONENT;
        public:
            Camera(void);
            ~Camera(void);

            Meta(Disable)
            graphics::GfxHND GetHandle(void) const;

            Meta(Disable)
            graphics::Camera &GetCamera(void) const;

        private:
            friend class RenderSystem;

            graphics::GfxHND m_handle;
        } Meta(Enable, DisplayName( "Camera" ));
    }
}