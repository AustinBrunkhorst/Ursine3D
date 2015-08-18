#include "UrsinePrecompiled.h"

#include "Shader.h"

#include <mutex>

namespace
{
    std::mutex mutex;

    unsigned getMaxTextureUnits(void)
    {
        std::lock_guard<std::mutex> lock( mutex );

        GLint maxUnits = 0;

        glGetIntegerv( GL_MAX_TEXTURE_COORDS_ARB, &maxUnits );

        return maxUnits;
    }
}

namespace Ursine
{
    Shader::CurrentTextureType Shader::CurrentTexture;

    Shader::Shader(void)
        : m_shaderProgram( 0 )
        , m_currentTexture( -1 )
    {
        UAssert(GLEW_ARB_shading_language_100 &&
                GLEW_ARB_shader_objects &&
                GLEW_ARB_vertex_shader &&
                GLEW_ARB_fragment_shader,
                "Hardware doesn't support shaders.");
    }

    Shader::~Shader(void)
    {
        if (m_shaderProgram)
            glDeleteObjectARB( m_shaderProgram );
    }

    void Shader::Bind(const Shader *shader)
    {
        if (shader && shader->m_shaderProgram)
        {
            // enable the program
            glUseProgramObjectARB( shader->m_shaderProgram );

            // bind the textures
            shader->bindTextures( );

            // bind the current texture
            if (shader->m_currentTexture != -1)
                glUniform1iARB( shader->m_currentTexture, 0 );
        }
        else
        {
            // bind none
            glUseProgramObjectARB( 0 );
        }
    }

    bool Shader::LoadFromFile(const std::string &filename, Type type)
    {
        auto source = gContentManager->LoadText( filename );

        if (type == Vertex)
            return compile( source->c_str( ), nullptr );

        return compile( nullptr, source->c_str( ) );
    }

    bool Shader::LoadFromFile(const std::string &vertexFile, const std::string &fragmentFile)
    {
        auto vertexSource = gContentManager->LoadText( vertexFile );
        auto fragmentSource = gContentManager->LoadText( fragmentFile );

        return compile( vertexSource->c_str( ), fragmentSource->c_str( ) );
    }

    bool Shader::LoadFromMemory(const std::string &source, Type type)
    {
        if (type == Vertex)
            return compile( source.c_str( ), nullptr );

        return compile( nullptr, source.c_str( ) );
    }

    bool Shader::LoadFromMemory(const std::string &vertexSource, const std::string &fragmentSource)
    {
        return compile( vertexSource.c_str( ), fragmentSource.c_str( ) );
    }

    void Shader::SetUniform(const std::string &name, float x)
    {
        if (!m_shaderProgram)
            return;

        // enable program
        auto program = glGetHandleARB( GL_PROGRAM_OBJECT_ARB );

        glUseProgramObjectARB( m_shaderProgram );

        // get uniform location and assign it new values
        auto location = getUniformLocation( name );

        if (location != -1)
        glUniform1fARB( location, x );

        // disable program
        glUseProgramObjectARB( program );
    }

    void Shader::SetUniform(const std::string &name, float x, float y)
    {
        if (!m_shaderProgram)
            return;

        // enable program
        auto program = glGetHandleARB( GL_PROGRAM_OBJECT_ARB );

        glUseProgramObjectARB( m_shaderProgram );

        // get uniform location and assign it new values
        auto location = getUniformLocation( name );

        if (location != -1)
            glUniform2fARB( location, x, y );

        // disable program
        glUseProgramObjectARB( program );
    }

    void Shader::SetUniform(const std::string &name, float x, float y, float z)
    {
        if (!m_shaderProgram)
            return;

        // enable program
        auto program = glGetHandleARB( GL_PROGRAM_OBJECT_ARB );

        glUseProgramObjectARB( m_shaderProgram );

        // get uniform location and assign it new values
        auto location = getUniformLocation( name );

        if (location != -1)
            glUniform3fARB( location, x, y, z );

        // disable program
        glUseProgramObjectARB( program );
    }

    void Shader::SetUniform(const std::string &name, float x, float y, float z, float w)
    {
        if (!m_shaderProgram)
            return;

        // enable program
        auto program = glGetHandleARB( GL_PROGRAM_OBJECT_ARB );

        glUseProgramObjectARB( m_shaderProgram );

        // get uniform location and assign it new values
        auto location = getUniformLocation( name );

        if (location != -1)
            glUniform4fARB( location, x, y, z, w );

        // disable program
        glUseProgramObjectARB( program );
    }

    void Shader::SetUniform(const std::string &name, const Vec2 &vector)
    {
        SetUniform( name, vector.X(), vector.Y() );
    }

    void Shader::SetUniform(const std::string &name, const Vec3 &vector)
    {
        SetUniform( name, vector.X(), vector.Y(), vector.Z() );
    }

    void Shader::SetUniform(const std::string &name, const Color &color)
    {
        SetUniform( name, color.r, color.g, color.b, color.a );
    }

    void Shader::SetUniform(const std::string &name, const Mat3 &transform)
    {
        if (!m_shaderProgram)
            return;

        // enable program
        auto program = glGetHandleARB( GL_PROGRAM_OBJECT_ARB );

        glUseProgramObjectARB( m_shaderProgram );

        // get uniform location and assign it new values
        GLint location = getUniformLocation( name );

        auto copy = transform;

        if (location != -1)
            // NOTE: The third param is as follows
            // GL_FALSE == matrix is in column major order
            // GL_TRUE  == matrix is in row major order
            glUniformMatrix3fvARB( location, 1, GL_FALSE, copy.GetFloatPtr( ) );

        // disable program
        glUseProgramObjectARB( program );
    }

    void Shader::SetUniform(const std::string &name, const Mat4 &transform)
    {
        if (!m_shaderProgram)
            return;

        // enable program
        auto program = glGetHandleARB( GL_PROGRAM_OBJECT_ARB );

        glUseProgramObjectARB( m_shaderProgram );

        // get uniform location and assign it new values
        GLint location = getUniformLocation( name );

        if (location != -1)
            glUniformMatrix4fvARB( location, 1, GL_FALSE, transform.GetFloatPtr( ) );

        // disable program
        glUseProgramObjectARB( program );
    }

    void Shader::SetUniform(const std::string &name, const Texture2D *texture)
    {
        if (!m_shaderProgram)
            return;

        auto location = getUniformLocation( name );

        if (location != -1)
        {
            // store the location -> texture mapping
            auto search = m_textures.find( location );

            if (search == m_textures.end( ))
            {
                // new entry, make sure there are enough texture units
                auto maxUnits = getMaxTextureUnits( );

                UAssert(m_textures.size() + 1 < maxUnits,
                    "Unable to use texture \"%s\"for shader: all available units are used.",
                    name.c_str())                ;

                m_textures[ location ] = texture;
            }
            else
            {
                // location already used, just replace the texture
                search->second = texture;
            }
        }
    }

    void Shader::SetUniform(const std::string &name, CurrentTextureType)
    {
        if (m_shaderProgram)
            m_currentTexture = getUniformLocation( name );
    }

    unsigned int Shader::GetProgramID(void) const
    {
        return m_shaderProgram;
    }

    bool Shader::compile(const char *vertexSource, const char *fragmentSource)
    {
        // destroy the shader if it was already created
        if (m_shaderProgram)
            glDeleteObjectARB( m_shaderProgram );

        // reset the internal state
        m_currentTexture = -1;

        m_textures.clear( );
        m_uniforms.clear( );

        // create the program
        m_shaderProgram = glCreateProgramObjectARB( );

        // create the vertex shader if needed
        if (vertexSource)
        {
            // create and compile the shader
            GLhandleARB vertexShader =
                glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );

            glShaderSourceARB( vertexShader, 1, &vertexSource, nullptr );
            glCompileShaderARB( vertexShader );

            // check the compile log
            GLint success;

            glGetObjectParameterivARB( vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success );

            if (success == GL_FALSE)
            {
                char log[1024];

                glGetInfoLogARB( vertexShader, sizeof(log), nullptr, log );

                UWarning("Error compiling vertex shader:\n%s", log);

                glDeleteObjectARB( vertexShader );
                glDeleteObjectARB( m_shaderProgram );

                m_shaderProgram = 0;

                return false;
            }

            // attach the shader to the program, and delete it (not needed anymore)
            glAttachObjectARB( m_shaderProgram, vertexShader );
            glDeleteObjectARB( vertexShader );
        }

        // create the fragment shader if needed
        if (fragmentSource)
        {
            // create and compile the shader
            GLhandleARB fragmentShader =
                glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );

            glShaderSourceARB( fragmentShader, 1, &fragmentSource, nullptr );
            glCompileShaderARB( fragmentShader );

            // check the compile log
            GLint success;

            glGetObjectParameterivARB( fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success );

            if (success == GL_FALSE)
            {
                char log[1024];

                glGetInfoLogARB( fragmentShader, sizeof(log), nullptr, log );

                UWarning("Error compiling fragment shader:\n%s", log);

                glDeleteObjectARB( fragmentShader );
                glDeleteObjectARB( m_shaderProgram );

                m_shaderProgram = 0;

                return false;
            }

            // attach the shader to the program, and delete it (not needed anymore)
            glAttachObjectARB( m_shaderProgram, fragmentShader );
            glDeleteObjectARB( fragmentShader );
        }

        // link the program
        glLinkProgramARB( m_shaderProgram );

        // check the link log
        GLint success;

        glGetObjectParameterivARB( m_shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success );

        if (success == GL_FALSE)
        {
            char log[1024];

            glGetInfoLogARB( m_shaderProgram, sizeof(log), nullptr, log );

            UWarning("Failed to link shader:\n%s", log);

            glDeleteObjectARB( m_shaderProgram );

            m_shaderProgram = 0;

            return false;
        }

        // force an OpenGL flush, so that the shader will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glFlush( );

        return true;
    }

    void Shader::bindTextures(void) const
    {
        auto it = m_textures.begin( );

        for (size_t i = 0; i < m_textures.size( ); ++i)
        {
            GLint index = static_cast<GLsizei>( i + 1 );

            glUniform1iARB( it->first, index );

            glActiveTextureARB( GL_TEXTURE0_ARB + index );

            Texture2D::Bind( it->second );

            ++it;
        }
    }

    int Shader::getUniformLocation(const std::string &name)
    {
        auto search = m_uniforms.find( name );

        // not found in cache
        if (search == m_uniforms.end( ))
        {
            // request the location from OpenGL
            int location = glGetUniformLocationARB( m_shaderProgram, name.c_str( ) );

            UAssert(location != -1,
                "Uniform not found in shader: %s", name.c_str());

            m_uniforms.emplace( name, location );

            return location;
        }


        // found in cache
        return search->second;
    }
}
