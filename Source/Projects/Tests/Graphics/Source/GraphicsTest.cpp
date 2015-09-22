#include "Precompiled.h"

#include "GraphicsTest.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>
#include <GfxAPI.h>

#include <Color.h>
#include <DirectXMath.h>

using namespace ursine;

namespace
{
    //const auto kGraphicsTestUIEntryPoint = "file:///Assets/Test.html";
    const auto kGraphicsTestUIEntryPoint = "www.google.com";

    const auto kDefaultWindowWidth = 1920;
    const auto kDefaultWindowHeight = 1080;

    void onResize(int width, int height)
    {
        // ::OPENGL
        /*glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );

        glViewport( 0, 0, width, height );
        glOrtho( 0, width, 0, height, -100.0f, 100.0f );*/

        ursine::Application::Instance->GetCoreSystem<ursine::GfxAPI>( )->Resize( width, height );
    }
}

CORE_SYSTEM_DEFINITION( GraphicsTest );

GraphicsTest::GraphicsTest(void)
    : m_mainWindow( nullptr )
{
    
}

GraphicsTest::~GraphicsTest(void)
{
    // ::OPENGL
    //SDL_GL_DeleteContext( m_glContext );

    delete m_mainWindow;
}

void GraphicsTest::OnInitialize(void)
{
    auto *app = ursine::Application::Instance;

    app->Connect(
        ursine::APP_UPDATE, 
        this, 
        &GraphicsTest::onAppUpdate 
    );

    auto *windowManager = app->GetCoreSystem<ursine::WindowManager>( );
    auto *uiManager = app->GetCoreSystem<ursine::UIManager>( );

    m_mainWindow = windowManager->AddWindow(
        "Ursine3D Editor", 
        { 0, 0 }, 
        { static_cast<float>( kDefaultWindowWidth ), static_cast<float>( kDefaultWindowHeight ) },
        SDL_WINDOW_RESIZABLE
    );

    m_mainWindow->Listener( this )
        .On( ursine::WINDOW_RESIZE, &GraphicsTest::onMainWindowResize );
  
    m_mainWindow->SetLocationCentered( );
    m_mainWindow->Show( true );

    /////////////////////////////////////////////////////////////////
    //make graphics manager
    auto *gfxManager = app->GetCoreSystem<ursine::GfxAPI>( );

    HWND handle = reinterpret_cast<HWND>((m_mainWindow->GetPlatformHandle( )));

    ursine::GfxConfig config;
    config.Fullscreen_ = false;
    config.HandleToWindow_ = &handle;
    config.ModelListPath_ = "Models/";
    config.ShaderListPath_ = "SHADER_BINARY/";
    config.TextureListPath_ = "Textures/";
    config.WindowWidth_ = kDefaultWindowWidth;
    config.WindowHeight_ = kDefaultWindowHeight;
    config.m_renderUI = false;
    config.Profile_ = false;
    config.debug = false;

    gfxManager->StartGraphics( config );

    m_ui = uiManager->CreateView( m_mainWindow, kGraphicsTestUIEntryPoint );

    m_ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );

    onResize( kDefaultWindowWidth, kDefaultWindowHeight );
}

void GraphicsTest::OnRemove(void)
{
    ursine::Application::Instance->Disconnect(
        ursine::APP_UPDATE, 
        this, 
        &GraphicsTest::onAppUpdate 
    );

    m_mainWindow->Listener( this )
        .Off( ursine::WINDOW_RESIZE, &GraphicsTest::onMainWindowResize );

    m_ui->Close( );
}

void GraphicsTest::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS(ursine::Application, ursine::EventArgs);

    // ::OPENGL
    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // @@@ TODO:
    // Update Scene
    // Render Tools
    // Render Editor UIView

    auto gfx = ursine::Application::Instance->GetCoreSystem<ursine::GfxAPI>( );

    static GFXHND prim, obj, floor, point, skybox;
    static GFXHND vp;
    static GFXHND cam;
    static GFXHND lights[ 10 ];
    static GFXHND cubes[ 10 ];
    static bool ran = false;
    static float dt = 0;

    DirectX::XMFLOAT3 colors[ 10 ] = {
      DirectX::XMFLOAT3(0, 0, 1),
      DirectX::XMFLOAT3(0, 1, 0),
      DirectX::XMFLOAT3(0, 1, 1),
      DirectX::XMFLOAT3(1, 0, 0),
      DirectX::XMFLOAT3(1, 0, 1),
      DirectX::XMFLOAT3(1, 1, 0),
      DirectX::XMFLOAT3(1, 1, 1),
      DirectX::XMFLOAT3(0, 0, 1),
      DirectX::XMFLOAT3(0, 1, 0),
      DirectX::XMFLOAT3(1, 0, 0),
    };

    dt += 0.016;

    if(!ran)
    {
      ran = true;
      prim = gfx->RenderableMgr.AddRenderable( RENDERABLE_PRIMITIVE );
      obj = gfx->RenderableMgr.AddRenderable( RENDERABLE_MODEL3D );
      skybox = gfx->RenderableMgr.AddRenderable( RENDERABLE_MODEL3D );
      floor = gfx->RenderableMgr.AddRenderable( RENDERABLE_MODEL3D );
      point = gfx->RenderableMgr.AddRenderable( RENDERABLE_POINT_LIGHT );

      for (int x = 0; x < 10; ++x)
      {
        lights[ x ] = gfx->RenderableMgr.AddRenderable( RENDERABLE_POINT_LIGHT );
        gfx->RenderableMgr.GetPointLight( lights[ x ] ).SetRadius( 5 );
        gfx->RenderableMgr.GetPointLight( lights[ x ] ).SetColor( Color( SVec3( colors[x] ) ) );

        cubes[x] = gfx->RenderableMgr.AddRenderable( RENDERABLE_MODEL3D );
      }

      gfx->RenderableMgr.GetModel3D( obj ).SetModel( "Pedestal" );
      gfx->RenderableMgr.GetModel3D( floor ).SetModel( "Cube" );
      gfx->RenderableMgr.GetModel3D( floor ).SetModel( "Cube" );
      gfx->RenderableMgr.GetModel3D( skybox ).SetModel( "Skybox" );
      
      gfx->RenderableMgr.GetModel3D( floor ).SetWorldMatrix( SMat4( DirectX::XMMatrixScaling( 10, 1, 10 ) * DirectX::XMMatrixTranslation( 0, -1, 0 ) ) );
      gfx->RenderableMgr.GetModel3D( skybox ).SetWorldMatrix( SMat4( DirectX::XMMatrixScaling( 20.f, 20.f, 20.f ) ) );

      gfx->RenderableMgr.GetPointLight( point ).SetPosition( 1, 1, -1 );
      gfx->RenderableMgr.GetPointLight( point ).SetRadius( 5 );
      gfx->RenderableMgr.GetPointLight( point ).SetColor( 1, 1, 1 );

      gfx->RenderableMgr.GetPrimitive( prim ).SetType( ursine::Primitive::PRIM_SPHERE );
      gfx->RenderableMgr.GetPrimitive( prim ).SetRadius( 1.1 );
      gfx->RenderableMgr.GetPrimitive( prim ).SetWidth( 1 );
      gfx->RenderableMgr.GetPrimitive( prim ).SetHeight( 1 );
      gfx->RenderableMgr.GetPrimitive( prim ).SetDepth( 1 );
      gfx->RenderableMgr.GetPrimitive( prim ).SetColor( 1, 1, 1, 1 );
      
      vp = gfx->ViewportMgr.CreateViewport( kDefaultWindowWidth, kDefaultWindowHeight );
      gfx->ViewportMgr.GetViewport( vp ).SetPosition( 0, 0 );
      //gfx->ViewportMgr.GetViewport( vp ).SetRenderMode( VIEWPORT_RENDER_FORWARD );
      cam = gfx->CameraMgr.AddCamera( );

      gfx->ViewportMgr.GetViewport(vp).SetViewportCamera( cam );
	  gfx->ViewportMgr.GetViewport(vp).SetRenderMode(ViewportRenderMode::VIEWPORT_RENDER_FORWARD);

      gfx->CameraMgr.GetCamera( cam ).SetPosition( SVec3( DirectX::XMFLOAT3( 0, 0, -10 ) ) );
      gfx->CameraMgr.GetCamera( cam ).LookAtPoint( SVec3( DirectX::XMFLOAT3( 0, 0, 0 ) ) );

      gfx->DrawingMgr.SetColor( 1, 1, 1, 1 );
      gfx->DrawingMgr.SetSize( 10 );
    }

    gfx->RenderableMgr.GetModel3D( obj ).SetWorldMatrix( SMat4( DirectX::XMMatrixRotationRollPitchYaw( 3.14f / 2, dt, 0 ) * DirectX::XMMatrixScaling( 0.0025, 0.0025, 0.0025 ) * DirectX::XMMatrixTranslation( 0, 1.1, 0 ) ) );

    float angle = 0;
    for (int x = 0; x < 10; ++x)
    {
      float radius = 5 + 3 * cos( dt );
      gfx->RenderableMgr.GetPointLight( lights[x] ).SetPosition( cosf( (dt + angle) ) * radius, 1 + cosf( (dt + angle) * 2 ) * 0.75, sinf( (dt + angle) ) * radius );
      gfx->DrawingMgr.SetColor( colors[ x ].x, colors[ x ].y, colors[ x ].z, 1 );
      gfx->DrawingMgr.DrawPoint( cosf( (dt + angle) ) * radius, 1 + cosf( (dt + angle) * 2 ) * 0.75, sinf( (dt + angle) ) * radius );

      gfx->RenderableMgr.GetPointLight( lights[ x ] ).SetRadius( 6 );

      gfx->RenderableMgr.GetModel3D( cubes[ x ] ).SetWorldMatrix( SMat4( DirectX::XMMatrixRotationRollPitchYaw( cos( angle - dt ), sin( angle - dt ), -cosf( angle - dt ) ) * DirectX::XMMatrixTranslation( cosf( -angle - dt ) * radius, 2 + cosf( (dt + angle) * 2 ), sinf( -angle - dt ) * radius ) ) );
      angle += 6.28 / 10;
    }
    
	gfx->DrawingMgr.SetSize(20);
	gfx->DrawingMgr.SetColor(1, 0, 0, 1);
	gfx->DrawingMgr.DrawPoint(0, 0, 0);

    gfx->BeginScene( );
    //gfx->RenderObject( prim );
    gfx->RenderObject( obj );
    gfx->RenderObject( floor );
    //gfx->RenderObject( point );
    gfx->RenderObject( skybox );

    for (int x = 0; x < 10; ++x)
    {
      gfx->RenderObject( lights[ x ] );
      gfx->RenderObject( cubes[ x ] );
    }

    gfx->RenderScene( 0.016f, vp ); //very back
    gfx->EndScene( );

    m_ui->Draw( );

    // ::OPENGL
    //SDL_GL_SwapWindow( m_mainWindow->GetHandle( ) );
}

void GraphicsTest::onMainWindowResize(EVENT_HANDLER(ursine::Window))
{
    EVENT_ATTRS(ursine::Window, ursine::WindowResizeArgs);

    onResize( args->width, args->height );

    m_ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}
