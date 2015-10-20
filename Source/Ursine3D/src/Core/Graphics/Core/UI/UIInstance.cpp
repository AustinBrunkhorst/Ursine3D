#include "UrsinePrecompiled.h"

#include "UIInstance.h"

#include "GfxManager.h"

#define GfxManager reinterpret_cast<GfxManager*>(m_gfxmgr)

namespace ursine
{
    namespace graphics
    {
        void UIInstance::Initialize(
            ID3D11Device *device,
            ID3D11DeviceContext *context,
            DXCore::RenderTargetManager *rtmgr,
            void *gfx,
            RENDER_TARGETS target
        )
        {
            m_device = device;
            m_context = context;
            m_gfxmgr = (gfx);

            unsigned x, y;
            GfxManager->gfxInfo->GetDimensions( x, y );

            m_width = x;
            m_height = y;

            m_paintingPopup = false;

            m_mainTarget = GfxManager->textureManager->CreateDynamicTexture(x, y);
            m_popup = GfxManager->textureManager->CreateDynamicTexture(1, 1);
        }

        void UIInstance::Uninitialize(void)
        {
            m_device = nullptr;
            m_context = nullptr;
        }

        void UIInstance::Draw(GfxHND camera)
        {
            GfxManager->RenderDynamicTexture(m_mainTarget, 0, 0);
        }

        void UIInstance::DrawMain(void)
        {
            GfxManager->RenderDynamicTexture(m_mainTarget, 0,0);
            
            URSINE_TODO("remove hard-coded edge case");
            //this is VERY VERY VERY temporary. Just so you can see it works.
            if (m_popupRect.y != 0)
            {
                GfxManager->RenderDynamicTexture(m_popup, m_popupRect.x, m_popupRect.y);
            }
        }

        bool UIInstance::GetViewRect(
            CefRefPtr<CefBrowser> browser, 
            CefRect &bounds
        )
        {
            bounds.x = 0;
            bounds.y = 0;

            //auto &size = gWindowManager->GetSize( );
            unsigned x, y;
            GfxManager->gfxInfo->GetDimensions( x, y );

            m_width = x;
            m_height = y;

            bounds.width = static_cast<int>( m_width );
            bounds.height = static_cast<int>( m_height );

            return true;
        }

        void UIInstance::OnPopupShow(
            CefRefPtr<CefBrowser> browser, 
            bool show
        )
        {
            if (!show)
            {
                m_popupRect.Set( 0, 0, 0, 0 );
                m_originalPopupRect.Set( 0, 0, 0, 0 );

                browser->GetHost( )->Invalidate( PET_VIEW );
            }
        }

        void UIInstance::OnPopupSize(
            CefRefPtr<CefBrowser> browser, 
            const CefRect &bounds
        )
        {
            if (bounds.width <= 0 || bounds.height <= 0)
                return;

            m_originalPopupRect = bounds;
            m_popupRect = getPopupRectInView( m_originalPopupRect );
            GfxManager->textureManager->ResizeDynamicTexture(m_popup, bounds.width, bounds.height);
        }

        void UIInstance::OnPaint(
            CefRefPtr<CefBrowser> browser,
            CefRenderHandler::PaintElementType type,
            const CefRenderHandler::RectList &regions,
            const void *buffer,
            int width,
            int height
        )
        {
            if (type == PET_VIEW)
                paintView( browser, type, regions, buffer, width, height );
            else if (m_popupRect.width > 0 && m_popupRect.height > 0)
                paintPopup( browser, type, regions, buffer, width, height );

            if (!m_paintingPopup && type == PET_VIEW && !m_popupRect.IsEmpty( ))
            {
                m_paintingPopup = true;

                browser->GetHost( )->Invalidate( PET_POPUP );

                m_paintingPopup = false;
            }
        }

        void UIInstance::Resize(int width, int height)
        {
            m_width = width;
            m_height = height;

            GfxManager->textureManager->ResizeDynamicTexture(m_mainTarget, width, height);
        }

        CefRect UIInstance::getPopupRectInView(const CefRect &original)
        {
            auto rect( original );

            // if x or y are negative, move them to 0

            if (rect.x < 0)
                rect.x = 0;

            if (rect.y < 0)
                rect.y = 0;

            // if popup goes outside the view, try to reposition origin

            if (rect.x + rect.width > m_width)
                rect.x = m_width - rect.width;

            if (rect.y + rect.height > m_height)
                rect.y = m_height - rect.height;

            // if x or y became negative, move them to 0 again

            if (rect.x < 0)
                rect.x = 0;

            if (rect.y < 0)
                rect.y = 0;

            return rect;
        }

        //paint main ui
        void UIInstance::paintView(
            CefRefPtr<CefBrowser> browser,
            CefRenderHandler::PaintElementType type,
            const CefRenderHandler::RectList &regions,
            const void *buffer,
            int width,
            int height
        )
        {
            URSINE_TODO("handle cases w/ multiple rect lists");
            int skip_pixels = 0, x = m_popupRect.x;
            int skip_rows = 0, y = m_popupRect.y;
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

            GfxManager->RenderToDynamicTexture(width, height, buffer, w, h, m_mainTarget, 0, 0);
        }

        //paint a popup
        void UIInstance::paintPopup(
            CefRefPtr<CefBrowser> browser,
            CefRenderHandler::PaintElementType type,
            const CefRenderHandler::RectList &regions,
            const void *buffer,
            int width,
            int height
        )
        {
            int skip_pixels = 0, x = m_popupRect.x;
            int skip_rows = 0, y = m_popupRect.y;
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

            //destinationX/Y is 0, 0 for a reason
            GfxManager->RenderToDynamicTexture(width, height, buffer, w, h, m_popup, 0, 0);
        }
    }
}
