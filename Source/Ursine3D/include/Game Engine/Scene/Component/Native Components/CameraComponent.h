#pragma once

#include "Component.h"

#include "Camera.h"

#include "GFXAPIDefines.h"

#undef min
#undef max

struct Range : ursine::meta::MetaProperty
{
    META_OBJECT

public:
    float min;
    float max;

    Range(float min, float max)
        : min( min )
        , max( max ) { }
};

namespace ursine
{
    namespace ecs
    {
        class Camera : public Component
        {
            NATIVE_COMPONENT;
        public:

            Meta(Range(-10.0f, 25.0f))
            int potato;

            Camera(void);
            ~Camera(void);

            GFXHND GetHandle(void) const;
            GFXCamera &GetCamera(void) const;

        private:
            friend class RenderSystem;

            GFXHND m_handle;
        } Meta(Enable, DisplayName( "Camera" ));
    }
}