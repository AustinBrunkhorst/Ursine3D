/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Renderer.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <cef_render_handler.h>

namespace ursine
{
    namespace UI
    {
        class Renderer : public CefRenderHandler
        {
            GLuint _texture_id;

            int _width, _height;

            CefRect _popup_bounds;

            bool GetViewRect(CefRefPtr<CefBrowser> browser,
                CefRect &bounds) override;

            void OnPopupShow(CefRefPtr<CefBrowser> browser, 
                bool show) override;

            void OnPopupSize(CefRefPtr<CefBrowser> browser, 
                const CefRect &bounds) override;

            void OnPaint(CefRefPtr<CefBrowser> browser, 
                PaintElementType type, const RectList &regions, 
                const void *buffer, int width, int height) override;

            void paintView(CefRefPtr<CefBrowser> browser, 
                PaintElementType type, const RectList &regions, 
                const void *buffer, int width, int height);

            void paintPopup(CefRefPtr<CefBrowser> browser, 
                PaintElementType type, const RectList &regions,
                const void *buffer, int width, int height);
        public:
            Renderer(void);
            ~Renderer(void);

            void Draw(void);

            IMPLEMENT_REFCOUNTING(Renderer);
        };
    }
}