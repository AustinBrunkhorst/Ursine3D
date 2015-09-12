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

#include <GL/glew.h>

namespace ursine
{
    class Renderer : public CefRenderHandler
    {
    public:
        Renderer(void);
        ~Renderer(void);

        void Draw(void);

    private:
        GLuint m_textureID;

        int m_width, m_height;

        CefRect m_popupBounds;

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

        IMPLEMENT_REFCOUNTING(Renderer);
    };
}