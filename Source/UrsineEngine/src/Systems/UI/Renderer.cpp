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

#include "Renderer.h"

#include "WindowManager.h"

#include <GL/glew.h>

namespace Ursine
{
    namespace UI
    {
        Renderer::Renderer(void)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glGenTextures(1, &_texture_id);

            glBindTexture(GL_TEXTURE_2D, _texture_id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }

        Renderer::~Renderer(void)
        {
            glDeleteTextures(1, &_texture_id);
        }

        void Renderer::Draw(void)
        {
            URSINE_TODO("Move this to the graphics system.");

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, _texture_id);

            glBegin(GL_QUADS);
            glTexCoord2i(0, 0);
            glVertex2f(-1.0f, 1.0f);
            glTexCoord2i(0, 1);
            glVertex2f(-1.0f, -1.0f);
            glTexCoord2i(1, 1);
            glVertex2f(1.0f, -1.0f);
            glTexCoord2i(1, 0);
            glVertex2f(1.0f, 1.0f);
            glEnd();
        }

        bool Renderer::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &bounds)
        {
            bounds.x = 0;
            bounds.y = 0;

            auto &size = gWindowManager->GetSize();

            bounds.width = static_cast<int>(size.x);
            bounds.height = static_cast<int>(size.y);

            return true;
        }

        void Renderer::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
        {
            if (!show)
            {
                browser->GetHost( )->Invalidate( _popup_bounds, PET_VIEW );

                _popup_bounds.Set( 0, 0, 0, 0 );
            }
        }

        void Renderer::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect &bounds)
        {
            if (bounds.width <= 0 || bounds.height <= 0)
                return;

            _popup_bounds = bounds;
        }

        void Renderer::OnPaint(
            CefRefPtr<CefBrowser> browser,
            PaintElementType type,
            const RectList &regions,
            const void *buffer,
            int width,
            int height)
        {
            glEnable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, _texture_id);

            if (type == PET_VIEW)
                paintView(browser, type, regions, buffer, width, height);
            else if (_popup_bounds.width > 0 && _popup_bounds.height > 0)
                paintPopup(browser, type, regions, buffer, width, height);
        }

        void Renderer::paintView(
            CefRefPtr<CefBrowser> browser,
            PaintElementType type,
            const RectList &regions,
            const void *buffer,
            int width,
            int height)
        {
            int old_width = _width;
            int old_height = _height;

            _width = width;
            _height = height;

            glPixelStorei(GL_UNPACK_ROW_LENGTH, _width);

            if (old_width != _width || old_height != _height)
            {
                glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
                glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0,
                    GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
            }
            else
            {
                auto size = regions.size();

                for (unsigned i = 0; i < size; ++i)
                {
                    auto &region = regions[i];

                    glPixelStorei(GL_UNPACK_SKIP_PIXELS, region.x);
                    glPixelStorei(GL_UNPACK_SKIP_ROWS, region.y);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, region.x, region.y, region.width,
                        region.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
                }
            }

            // reset
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
            glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        }

        void Renderer::paintPopup(
            CefRefPtr<CefBrowser> browser,
            PaintElementType type,
            const RectList &regions,
            const void *buffer,
            int width,
            int height)
        {
            int skip_pixels = 0, x = _popup_bounds.x;
            int skip_rows = 0, y = _popup_bounds.y;
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

            if (x + w > _width)
                w -= x + w - _width;
            if (y + h > _height)
                h -= y + h - _height;

            // update the popup rectangle.
            glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
            glPixelStorei(GL_UNPACK_SKIP_PIXELS, skip_pixels);
            glPixelStorei(GL_UNPACK_SKIP_ROWS, skip_rows);
            glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA,
                GL_UNSIGNED_INT_8_8_8_8_REV, buffer);

            // reset
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
            glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        }
    }
}