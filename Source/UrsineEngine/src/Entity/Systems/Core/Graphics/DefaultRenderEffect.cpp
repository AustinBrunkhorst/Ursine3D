#include "UrsinePrecompiled.h"

#include "DefaultRenderEffect.h"

#include "RenderableComponent.h"

namespace ursine
{
    namespace ecs
    {
        DefaultRenderEffect::DefaultRenderEffect(void)
        {
            static const Vec2 quad[] = {
                Vec2(-1, -1),
                Vec2(1, -1),
                Vec2(1, 1),
                Vec2(-1, 1)
            };

            _shader.LoadFromFile("Assets/Shaders/simple_quad.vert", "Assets/Shaders/texture.frag");

            Shader::Bind(&_shader);

            glGenBuffers(1, &_buffer_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _buffer_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

            glGenVertexArrays(1, &_buffer_vao);
            glBindVertexArray(_buffer_vao);

            glBindBuffer(GL_ARRAY_BUFFER, _buffer_vbo);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

            // clear binds
            glBindVertexArray(0);

            Shader::Bind(nullptr);
        }

        DefaultRenderEffect::~DefaultRenderEffect(void)
        {
            
        }

        void DefaultRenderEffect::Bind(World *world)
        {
            _world_to_NDC = world->GetCamera().GetWorldToNDC();

            Texture2D::Bind(nullptr);

            glBindVertexArray(_buffer_vao);

            Shader::Bind(&_shader);
        }

        void DefaultRenderEffect::Render(Renderable *renderable)
        {
            auto owner = renderable->GetOwner();
            auto *transform = owner->GetTransform();
            auto texture = renderable->GetTexture();

            // we don't want to draw this guy
            if (!renderable->GetVisible() || !texture)
                return;

            // set the model view matrix
            _shader.SetUniform("world_to_NDC", _world_to_NDC);
            _shader.SetUniform("obj_to_world", transform->Matrix());

            glActiveTextureARB(GL_TEXTURE0_ARB);
            glBindTexture(GL_TEXTURE_2D, texture->GetID());
            _shader.SetUniform("tint", renderable->GetTint());

            glDrawArrays(GL_QUADS, 0, 4);
        }

        void DefaultRenderEffect::UnBind(void)
        {
            // unbind vertex array
            glBindVertexArray(0);

            Shader::Bind(nullptr);

            // reset to white
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
}