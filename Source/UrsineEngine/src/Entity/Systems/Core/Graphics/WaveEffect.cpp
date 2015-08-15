#include "UrsinePrecompiled.h"

#include "WaveEffect.h"
#include "RenderableComponent.h"

namespace Ursine
{
    namespace ECS
    {
        WaveEffect::WaveEffect(void)
            : _time(0.0f)
        {
            static const Vector2 quad[] = {
                Vector2(-1, -1),
                Vector2(1, -1),
                Vector2(1, 1),
                Vector2(-1, 1)
            };

            _shader.LoadFromFile("Assets/Shaders/simple_quad.vert", "Assets/Shaders/wave.frag");

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

        WaveEffect::~WaveEffect(void)
        {

        }

        void WaveEffect::Bind(World *world)
        {
            _world_to_NDC = world->GetCamera().GetWorldToNDC();
            _time += gApplication->GetDelta() * 0.05f;

            _time = Math::Wrap(_time, 0.0f, 1.0f);

            Texture2D::Bind(nullptr);

            glBindVertexArray(_buffer_vao);
        }

        void WaveEffect::Render(Renderable *renderable)
        {
            auto owner = renderable->GetOwner();
            auto *transform = owner->GetTransform();
            auto texture = renderable->GetTexture();

            // we don't want to draw this guy
            if (!renderable->GetVisible() || !texture)
                return;

            Shader::Bind(&_shader);

            // set the model view matrix
            _shader.SetUniform("world_to_NDC", _world_to_NDC);
            _shader.SetUniform("obj_to_world", transform->Matrix());

            glActiveTextureARB(GL_TEXTURE0_ARB);
            glBindTexture(GL_TEXTURE_2D, texture->GetID());

            _shader.SetUniform("center", 0.5f, 0.5f);
            _shader.SetUniform("time", Ease::SineInOut(_time));
            _shader.SetUniform("tint", renderable->GetTint());
            
            glDrawArrays(GL_QUADS, 0, 4);

            Shader::Bind(nullptr);
        }

        void WaveEffect::UnBind(void)
        {
            // unbind vertex array
            glBindVertexArray(0);

            Shader::Bind(nullptr);

            // reset to white
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);            
        }
    }
}