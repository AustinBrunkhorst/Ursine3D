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
            Meta(Enable)
            Camera(void);
            ~Camera(void);

            void OnInitialize(void) override;

            graphics::GfxHND GetHandle(void) const;
            graphics::Camera *GetCamera(void) const;

        private:
            friend class RenderSystem;

            graphics::GfxHND m_handle;
        } Meta(Enable, WhiteListMethods, DisplayName( "Camera" ));
    }
}