#include "UrsinePrecompiled.h"

#include "BlurEffect.h"

#include "RenderableComponent.h"

namespace Ursine
{
    namespace ECS
    {
        BlurEffect::BlurEffect(void)
        {
            static const Vec2 quad[] = {
                Vec2(-1, -1),
                Vec2(1, -1),
                Vec2(1, 1),
                Vec2(-1, 1)
            };

            // compile shaders
            _x_shader.LoadFromFile("Assets/Shaders/blur_x.vert", "Assets/Shaders/blur_x.frag");
            _y_shader.LoadFromFile("Assets/Shaders/blur_y.vert", "Assets/Shaders/blur_y.frag");

            Shader::Bind(&_x_shader);
            
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

            Shader::Bind(&_y_shader);

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

        BlurEffect::~BlurEffect(void)
        {

        }

        void BlurEffect::Bind(World *world)
        {
            // get the camera transform
            _world_to_ndc = world->GetCamera().GetWorldToNDC();

            Texture2D::Bind(nullptr);

            glBindVertexArray(_buffer_vao);
        }

        void BlurEffect::Render(Renderable *renderable)
        {
            auto texture = renderable->GetTexture();
            auto width = texture->GetSize().X();
            auto height = texture->GetSize().Y();

            // calculate our model view matrix
            Mat3 model_view = _world_to_ndc * renderable->GetOwner()->GetTransform()->Matrix();

            Shader::Bind(&_x_shader);

            // set the uniforms on the shader
            _x_shader.SetUniform("model_view", model_view);
            _x_shader.SetUniform("blur_size", 1.0f / width);

            // bind the texture to the shader's uniform location
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glBindTexture(GL_TEXTURE_2D, renderable->GetTexture()->GetID());

            glDrawArrays(GL_QUADS, 0, 4);

            Shader::Bind(&_y_shader);

            _y_shader.SetUniform("model_view", model_view);
            _y_shader.SetUniform("blur_size", 1.0f / height);
            
            // bind the texture to the shader's uniform location
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glBindTexture(GL_TEXTURE_2D, renderable->GetTexture()->GetID());

            glDrawArrays(GL_QUADS, 0, 4);
        }

        void BlurEffect::UnBind(void)
        {
            // unbind vertex array
            glBindVertexArray(0);

            // unbind the shader
            Shader::Bind(nullptr);
        }
    }
}
