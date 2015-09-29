#include "UrsinePrecompiled.h"
#include "GraphicsManager.h"
#include "GfxAPI.h"

namespace ursine
{
  CORE_SYSTEM_DEFINITION( GfxAPI );

  //privates
  struct GfxAPI::privateData
  {
    GraphicsCore *gfxCore_;
  };

  GfxAPI::GfxAPI(void)
  {

  }

  void GfxAPI::OnInitialize ( )
  {

  }

  void GfxAPI::OnRemove ( )
  {
    //uninit API managers
    ResourceMgr.Uninitialize( );
    RenderableMgr.Uninitialize( );
    CameraMgr.Uninitialize( );
    ViewportMgr.Uninitialize( );
    UIMgr.Uninitialize( );
    DrawingMgr.Uninitialize( );

    //uninit graphics as a whole
    m_privates->gfxCore_->Uninitialize( );

    //delete everything
    delete m_privates->gfxCore_;
    delete m_privates;
  }

  void GfxAPI::StartGraphics ( GfxConfig config )
  {
    //create privates
    m_privates = new privateData;

    //create gfx
    m_privates->gfxCore_ = new GraphicsCore( );
    m_privates->gfxCore_->Initialize( config );

    //create APIs
    ResourceMgr.Initialize( );
    ResourceMgr.SetPrivates( m_privates->gfxCore_->modelManager, m_privates->gfxCore_->textureManager );
    RenderableMgr.Initialize( );
    RenderableMgr.SetPrivates( m_privates->gfxCore_->renderableManager );
    CameraMgr.Initialize( );
    CameraMgr.SetPrivates( m_privates->gfxCore_->cameraManager );
    ViewportMgr.Initialize( );
    ViewportMgr.SetPrivates( m_privates->gfxCore_->viewportManager );
    UIMgr.Initialize( );
    UIMgr.SetPrivates( m_privates->gfxCore_->uiManager );
    DrawingMgr.Initialize( );
    DrawingMgr.SetPrivates( m_privates->gfxCore_->drawingManager );
  }

  void GfxAPI::StartFrame ( )
  {
    m_privates->gfxCore_->StartFrame( );
  }

  void GfxAPI::RenderObject(GFXHND handle)
  {
    m_privates->gfxCore_->Render(handle);
  }

  void GfxAPI::BeginScene( )
  {
    m_privates->gfxCore_->BeginScene( );
  }

  void GfxAPI::RenderScene( float dt, GFXHND camera )
  {
    m_privates->gfxCore_->RenderScene(dt, camera );
  }

  void GfxAPI::EndScene( )
  {
    m_privates->gfxCore_->EndScene( );
  }

  void GfxAPI::EndFrame ( )
  {
    m_privates->gfxCore_->EndFrame( );
  }

  void GfxAPI::Resize ( int width, int height )
  {
    m_privates->gfxCore_->Resize( width, height );
  }

    void GfxAPI::SetGameViewport(GFXHND vp)
  {
      m_privates->gfxCore_->SetGameViewport( vp );
  }
}