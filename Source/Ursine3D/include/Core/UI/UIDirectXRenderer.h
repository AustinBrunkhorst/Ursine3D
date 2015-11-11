#pragma once

#include <cef_render_handler.h>

#include "GfxAPI.h"

namespace ursine
{
    class UIDirectXRenderer : public CefRenderHandler
    {
    public:
        UIDirectXRenderer(void);
        virtual ~UIDirectXRenderer(void);

        void Draw(GfxHND camera);
        void DrawMain(void);

    protected:
        void resize(const int width, const int height);

    private:
        graphics::GfxAPI *m_gfxMgr;
        GfxHND m_uiHandle;

        bool GetViewRect(
            CefRefPtr<CefBrowser> browser, CefRect &bounds
        ) override;

        void OnPopupShow(
            CefRefPtr<CefBrowser> browser,
            bool show
        ) override;

        void OnPopupSize(
            CefRefPtr<CefBrowser> browser,
            const CefRect &bounds
        ) override;

        void OnPaint(
            CefRefPtr<CefBrowser> browser,
            PaintElementType type, 
            const RectList &regions,
            const void *buffer, 
            int width, 
            int height
        ) override;

        IMPLEMENT_REFCOUNTING( UIDirectXRenderer );
    };
}
