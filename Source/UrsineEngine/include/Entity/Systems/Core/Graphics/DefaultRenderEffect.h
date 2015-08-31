#pragma once

#include "RenderEffect.h"
#include "Shader.h"

namespace Ursine
{
    namespace ECS
    {
        class DefaultRenderEffect : public RenderEffect
        {
            Shader _shader;

            SMat3 _world_to_NDC;

            GLuint _buffer_vao;
            GLuint _buffer_vbo;

        public:
            DefaultRenderEffect(void);
            ~DefaultRenderEffect(void);

            void Bind(World *world) override;
            void Render(Renderable *renderable) override;
            void UnBind(void) override;

			ALLOW_ALIGNED_ALLOC(16)
        };
    }
}