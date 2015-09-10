#pragma once

#include "Texture2D.h"

namespace ursine
{
    class Shader : public utils::NoCopy
    {
    public:
        enum Type
        {
            // Vertex shader
            Vertex,
            // Fragment (pixel) shader
            Fragment
        };

        struct CurrentTextureType {};

        Shader(void);
        ~Shader(void);

        static CurrentTextureType CurrentTexture;

        static void Bind(const Shader *shader);

        ////////////////////////////////////////////////////////////////////////
        // Load Methods
        ////////////////////////////////////////////////////////////////////////

        bool LoadFromFile(const std::string &filename, Type type);
        bool LoadFromFile(const std::string &vertexFile, const std::string &fragmentFile);

        bool LoadFromMemory(const std::string &source, Type type);
        bool LoadFromMemory(const std::string &vertexSource, const std::string &fragmentSource);

        void SetUniform(const std::string &name, float x);
        void SetUniform(const std::string &name, float x, float y);
        void SetUniform(const std::string &name, float x, float y, float z);
        void SetUniform(const std::string &name, float x, float y, float z, float w);

        void SetUniform(const std::string &name, const Vec2 &vector);
        void SetUniform(const std::string &name, const SVec3 &vector);

        void SetUniform(const std::string &name, const Color &color);

        void SetUniform(const std::string &name, const SMat3 &transform);
        void SetUniform(const std::string &name, const SMat4 &transform);

        void SetUniform(const std::string &name, const Texture2D *texture);
        void SetUniform(const std::string &name, CurrentTextureType);

        unsigned int GetProgramID(void) const;

    private:
        bool compile(const char *vertexSource, const char *fragmentSource);

        void bindTextures(void) const;

        int getUniformLocation(const std::string &name);

        ////////////////////////////////////////////////////////////////////////
        // Types
        ////////////////////////////////////////////////////////////////////////

        typedef std::unordered_map<int, const Texture2D*> TextureMap;
        typedef std::unordered_map<std::string, int> UniformMap;

        ////////////////////////////////////////////////////////////////////////
        // Members
        ////////////////////////////////////////////////////////////////////////

        // OpenGL identifier for the program
        unsigned int m_shaderProgram;

        // location of the current texture in the shader
        int m_currentTexture;

        // texture variables in the shader, mapped to their location
        TextureMap m_textures;

        // parameters location cache
        UniformMap m_uniforms;
    };
}
