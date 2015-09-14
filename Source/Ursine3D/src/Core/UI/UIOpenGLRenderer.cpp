/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Renderer.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UIOpenGLRenderer.h"

#include <GL/glew.h>

namespace ursine
{
    UIOpenGLRenderer::UIOpenGLRenderer(void)
        : m_width( 0 )
        , m_height( 0 )
    {
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

        glGenTextures( 1, &m_textureID );

        glBindTexture( GL_TEXTURE_2D, m_textureID );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    }

    UIOpenGLRenderer::~UIOpenGLRenderer(void)
    {
        glDeleteTextures( 1, &m_textureID );
    }

    void UIOpenGLRenderer::Draw(void)
    {
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, m_textureID );

        glBegin( GL_QUADS );
        glTexCoord2i( 0, 0 ); glVertex2i( m_viewport.x, m_viewport.height );
        glTexCoord2i( 0, 1 ); glVertex2i( m_viewport.x, m_viewport.y );
        glTexCoord2i( 1, 1 ); glVertex2i( m_viewport.width, m_viewport.y );
        glTexCoord2i( 1, 0 ); glVertex2i( m_viewport.width, m_viewport.height );
        glEnd( );
    }

    bool UIOpenGLRenderer::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &bounds)
    {
        bounds = m_viewport;

        return true;
    }

    void UIOpenGLRenderer::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
    {
        if (!show)
        {
            URSINE_TODO( "test" );
            browser->GetHost( )->Invalidate( PET_VIEW );

            m_popupBounds.Set( 0, 0, 0, 0 );
        }
    }

    void UIOpenGLRenderer::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect &bounds)
    {
        if (bounds.width <= 0 || bounds.height <= 0)
            return;

        m_popupBounds = bounds;
    }

    void UIOpenGLRenderer::OnPaint(
        CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList &regions,
        const void *buffer,
        int width,
        int height)
    {
        glEnable( GL_BLEND );
        glEnable( GL_TEXTURE_2D );

        glBindTexture( GL_TEXTURE_2D, m_textureID );

        if (type == PET_VIEW)
            paintView( browser, type, regions, buffer, width, height );
        else if (m_popupBounds.width > 0 && m_popupBounds.height > 0)
            paintPopup( browser, type, regions, buffer, width, height );
    }

    void UIOpenGLRenderer::paintView(
        CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList &regions,
        const void *buffer,
        int width,
        int height)
    {
        int old_width = m_width;
        int old_height = m_height;

        m_width = width;
        m_height = height;

        glPixelStorei( GL_UNPACK_ROW_LENGTH, m_width );

        if (old_width != m_width || old_height != m_height)
        {
            glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
            glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0,
                GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer );
        }
        else
        {
            auto size = regions.size( );

            for (unsigned i = 0; i < size; ++i)
            {
                auto &region = regions[ i ];

                glPixelStorei( GL_UNPACK_SKIP_PIXELS, region.x );
                glPixelStorei( GL_UNPACK_SKIP_ROWS, region.y );
                glTexSubImage2D( GL_TEXTURE_2D, 0, region.x, region.y, region.width,
                    region.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer );
            }
        }

        // reset
        glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
        glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
        glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
    }

    void UIOpenGLRenderer::paintPopup(
        CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList &regions,
        const void *buffer,
        int width,
        int height)
    {
        int skip_pixels = 0, x = m_popupBounds.x;
        int skip_rows = 0, y = m_popupBounds.y;
        int w = width;
        int h = height;

        // adjust the popup to fit inside the view.
        if (x < 0)
        {
            skip_pixels = -x;
            x = 0;
        }

        if (y < 0)
        {
            skip_rows = -y;
            y = 0;
        }

        if (x + w > m_width)
            w -= x + w - m_width;
        if (y + h > m_height)
            h -= y + h - m_height;

        // update the popup rectangle.
        glPixelStorei( GL_UNPACK_ROW_LENGTH, width );
        glPixelStorei( GL_UNPACK_SKIP_PIXELS, skip_pixels );
        glPixelStorei( GL_UNPACK_SKIP_ROWS, skip_rows );
        glTexSubImage2D( GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA,
            GL_UNSIGNED_INT_8_8_8_8_REV, buffer );

        // reset
        glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
        glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
        glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
    }
}
