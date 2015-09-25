#include "UrsinePrecompiled.h"

#include "UIManager.h"

#include "GraphicsManager.h"

#define GfxManager reinterpret_cast<GraphicsCore*>(m_gfxmgr)

namespace ursine
{
  void GfxUIManager::Initialize( ID3D11Device *device, ID3D11DeviceContext *context, DXCore::RenderTargetManager *rtmgr, void *gfx )
  {
    m_device = device;
    m_context = context;
    m_gfxmgr = (gfx);

    m_rtManager = rtmgr;

    unsigned x, y;
    GfxManager->gfxInfo->GetDimensions( x, y );

    m_width = x;
    m_height = y;
  }

  void GfxUIManager::Uninitialize( )
  {
    m_device = nullptr;
    m_context = nullptr;
  }

  bool GfxUIManager::GetViewRect( CefRefPtr<CefBrowser> browser, CefRect &bounds )
  {
    bounds.x = 0;
    bounds.y = 0;

    //auto &size = gWindowManager->GetSize( );
    unsigned x, y;
    GfxManager->gfxInfo->GetDimensions( x, y );

    m_width = x;
    m_height = y;

    bounds.width = static_cast<int>(m_width);
    bounds.height = static_cast<int>(m_height);

    return true;
  }

  void GfxUIManager::OnPopupShow( CefRefPtr<CefBrowser> browser, bool show )
  {
    if (!show)
    {
      browser->GetHost( )->Invalidate( PET_VIEW );

      m_popm_upbounds.Set( 0, 0, 0, 0 );
    }
  }

  void GfxUIManager::OnPopupSize( CefRefPtr<CefBrowser> browser, const CefRect &bounds )
  {
    if (bounds.width <= 0 || bounds.height <= 0)
      return;

    m_popm_upbounds = bounds;
  }

  void GfxUIManager::OnPaint(
    CefRefPtr<CefBrowser> browser,
    PaintElementType type,
    const RectList &regions,
    const void *buffer,
    int width,
    int height )
  {

    if (type == PET_VIEW)
      paintView( browser, type, regions, buffer, width, height );
    else if (m_popm_upbounds.width > 0 && m_popm_upbounds.height > 0)
      paintPopup( browser, type, regions, buffer, width, height );
  }

  //paint main ui
  void GfxUIManager::paintView(
    CefRefPtr<CefBrowser> browser,
    PaintElementType type,
    const RectList &regions,
    const void *buffer,
    int width,
    int height )
  {

    //@Matt temp
    paintPopup( browser, type, regions, buffer, width, height );
  }

  //paint a popup
  void GfxUIManager::paintPopup(
    CefRefPtr<CefBrowser> browser,
    PaintElementType type,
    const RectList &regions,
    const void *buffer,
    int width,
    int height )
  {
    int skip_pixels = 0, x = m_popm_upbounds.x;
    int skip_rows = 0, y = m_popm_upbounds.y;
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

    //set up description
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = w;
    desc.Height = h;
    desc.MipLevels = desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // dammmnnnn
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    //set up resource
    D3D11_SUBRESOURCE_DATA subrsc;
    subrsc.pSysMem = buffer;
    subrsc.SysMemPitch = width * 4; //length of one line in bytes, 32 bit color
    subrsc.SysMemSlicePitch = 0;

    //create the texture
    ID3D11Texture2D *tex;
    HRESULT hr = m_device->CreateTexture2D( &desc, &subrsc, &tex );

    UAssert( hr == S_OK, "Failed to create UI texture!" );

    //this doesn't work for now
    //define the box of the texture
    D3D11_BOX box;    //this box is the taken from the SOURCE texture
    box.back = 1;     //this might need SERIOUS changes
    box.front = 0;

    box.left = 0;
    box.top = 0;

    box.right = w;
    box.bottom = h;

    //now that we have the texture, we need to write it to the render target
    DXCore::RenderTarget *rt = m_rtManager->GetRenderTarget( RENDER_TARGET_UI );
    m_context->CopySubresourceRegion( rt->TextureMap, 0, x, y, 0, tex, 0, &box );

    RELEASE_RESOURCE( tex );
  }

  void GfxUIManager::Resize ( int width, int height )
  {
    m_width = width;
    m_height = height;

    m_rtManager->ResizeUI( width, height );
  }
}