/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Texture2D.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Texture2D.h"

#include "ScopedTextureBind.h"

#include <GL/glew.h>
#include <SDL_image.h>

#include <mutex>

namespace
{
    std::mutex mutex;

    // unique ID generator for render target cache functionality
    uint64 getUniqueID(void)
    {
        std::lock_guard<std::mutex> lock( mutex );

        // start at 1
        static uint64 id = 1;

        return id++;
    }

    // gets a valid texture dimension size based on if the hardware supports
    // non power-of-two sizes
    int getValidSize(int size)
    {
        // supports non power-of-two
        if (GLEW_ARB_texture_non_power_of_two)
            return size;

        int nearest = 1;

        while (nearest < size)
            nearest <<= 1;

        return nearest;
    }

    // maximum supported texture size by the hardware
    int getMaxSize(void)
    {
        GLint size;

        glGetIntegerv( GL_MAX_TEXTURE_SIZE, &size );

        return size;
    }
}

namespace ursine
{
    Texture2D::Texture2D(const std::string &path)
        : m_uniqueID( getUniqueID( ) )
        , m_filterMode( TF_LINEAR )
        , m_wrapMode( TW_REPEAT ) { }

    Texture2D::~Texture2D(void)
    {
        glDeleteTextures( 1, &m_id );
    }

    Texture2D *Texture2D::FromFile(const std::string &filename)
    {
        SDL_Surface *surface = IMG_Load( filename.c_str( ) );

        UAssert(surface,
            "Failed to load texture.\n%s",
            filename.c_str())

        Vec2 realSize {
            static_cast<float>(surface->w),
            static_cast<float>(surface->h)
        };

        Vec2 size {
            static_cast<float>(getValidSize(surface->w)),
            static_cast<float>(getValidSize(surface->h))
        };

        auto maxSize = getMaxSize( );

        UAssert(size.X() < maxSize && size.Y() < maxSize,
            "Texture size too large (%i, %i) max is (%i, %i).",
            static_cast<int>(size.X()),
            static_cast<int>(size.Y()),
            maxSize,
            maxSize);

        auto texture = new Texture2D( filename );

        texture->m_size = size;
        texture->m_realSize = realSize;

        // default alignment
        glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

        glGenTextures( 1, &texture->m_id );

        ScopedTextureBind bind_save;

        glBindTexture( GL_TEXTURE_2D, texture->m_id );

        // default is linear
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // default is repeated
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        GLenum format = 0;

        if (surface->format->BytesPerPixel == 4)
        {
            if (surface->format->Rmask == 0x000000FF)
                format = GL_RGBA;
            else
                format = GL_BGRA;
        }
        else if (surface->format->BytesPerPixel == 3)
        {
            if (surface->format->Rmask == 0x000000FF)
                format = GL_RGB;
            else
                format = GL_BGR;
        }
        else
        {
            UError("Unsupported texture format.\n%s",
                filename.c_str( ) );
        }

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
            format, GL_UNSIGNED_BYTE, surface->pixels );

        SDL_FreeSurface( surface );

        return texture;
    }

    void Texture2D::Bind(const Texture2D *texture)
    {
        if (texture && texture->m_id)
        {
            glBindTexture( GL_TEXTURE_2D, texture->m_id );
        }
        else
        {
            // bind no texture
            glBindTexture( GL_TEXTURE_2D, 0 );

            // reset the texture matrix
            glMatrixMode( GL_TEXTURE );
            glLoadIdentity( );

            // go back to model view mode
            glMatrixMode( GL_MODELVIEW );
        }
    }

    GLuint Texture2D::GetID(void) const
    {
        return m_id;
    }

    uint64 Texture2D::GetUniqueID(void) const
    {
        return m_uniqueID;
    }

    const Vec2 &Texture2D::GetSize(void) const
    {
        return m_size;
    }

    TextureFilterMode Texture2D::GetFilterMode(void) const
    {
        return m_filterMode;
    }

    void Texture2D::SetFilterMode(TextureFilterMode mode)
    {
        if (mode != m_filterMode)
        {
            m_filterMode = mode;

            if (m_id)
            {
                // make sure that the current texture binding will be preserved
                ScopedTextureBind bindSaver;

                static const GLint modeMap [] = {
                    GL_NEAREST,
                    GL_LINEAR
                };

                glBindTexture( GL_TEXTURE_2D, m_id );

                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, modeMap[ mode ] );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, modeMap[ mode ] );
            }
        }
    }

    TextureWrapMode Texture2D::GetWrapMode(void) const
    {
        return m_wrapMode;
    }

    void Texture2D::SetWrapMode(TextureWrapMode mode)
    {
        if (mode != m_wrapMode)
        {
            m_wrapMode = mode;

            if (m_id)
            {
                // make sure that the current texture binding will be preserved
                ScopedTextureBind bindSaver;

                static const GLint modeMap [] = {
                    GL_REPEAT,
                    GL_MIRRORED_REPEAT,
                    GL_CLAMP_TO_EDGE
                };

                glBindTexture( GL_TEXTURE_2D, m_id );

                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, modeMap[ mode ] );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, modeMap[ mode ] );
            }
        }
    }
}
