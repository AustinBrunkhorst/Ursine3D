#include <Windows.h>

#include "Graphics\GfxAPI.h"

LRESULT CALLBACK WndProc( HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam )
{
  int i = 0;
  HDC dc;
  PAINTSTRUCT ps;


  switch (umessage)
  {
  case WM_PAINT:
    dc = BeginPaint( hwnd, &ps );
    EndPaint( hwnd, &ps );
    break;
  case WM_KEYDOWN:
    //switch (wparam)
    //{
    //  printf("%i\n", wparam);
    //}
    break;
  case WM_DESTROY:
  case WM_CLOSE:
    exit( 0 );
    break;
  default:
    return DefWindowProc( hwnd, umessage, wparam, lparam );
    break;
  }

  return 0;
}

HWND InitWindow( unsigned width, unsigned height )
{
  WNDCLASSEX wc;
  HINSTANCE HInstance_;
  LPCWSTR ApplicationName_;
  HWND HWND_;
  int posX, posY;
  //Give the application a name.

  //Get the instance of this application.
  HInstance_ = GetModuleHandle( NULL );

  ApplicationName_ = L"Graphics3DProto";

  //Setup the windows class with default settings.
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = HInstance_;
  wc.hIcon = LoadIcon( HInstance_, NULL );
  wc.hIconSm = wc.hIcon;
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
  wc.lpszMenuName = NULL;
  wc.lpszClassName = ApplicationName_;
  wc.cbSize = sizeof( WNDCLASSEX );

  //Register the window class.
  RegisterClassEx( &wc );

  //Determine the resolution of the clients desktop screen.
  int screenWidth = GetSystemMetrics( SM_CXSCREEN );
  int screenHeight = GetSystemMetrics( SM_CYSCREEN );

  //pos to 0
  posX = posY = 0;

  int windowParam = WS_OVERLAPPEDWINDOW; //WS_POPUP
  int params = 0;

  //Setup the screen settings depending on whether it is running in full screen or in windowed mode.

  //set params for window
  params = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

  screenWidth = width;
  screenHeight = height;

  //Place the window in the middle of the screen.
  posX = (GetSystemMetrics( SM_CXSCREEN ) - screenWidth) / 2;
  posY = (GetSystemMetrics( SM_CYSCREEN ) - screenHeight) / 2;

  //Create the window with the screen settings and get the handle to it.
  HWND_ = CreateWindowEx( WS_EX_APPWINDOW, ApplicationName_, ApplicationName_,
    params,
    posX, posY, screenWidth, screenHeight, NULL, NULL, HInstance_, NULL );

  //Bring the window up on the screen and set it as main focus.
  ShowWindow( HWND_, SW_SHOW );
  SetForegroundWindow( HWND_ );
  SetFocus( HWND_ );

  return HWND_;
}

//position of the camera
float cameraPosition[ 4 ] = { 0, 0, 10, 1 };
//vertical speed of camera
float verticalSpeed = 15.f;
//horizontal speed of camera
float horizontalSpeed = 15.f;

void updateCamera(void)
{
  //forward/back
  if (GetAsyncKeyState( 'W' ) & 0x8000)
  {
    cameraPosition[ 2 ] -= horizontalSpeed / 1000.f;
  }
  if (GetAsyncKeyState( 'S' ) & 0x8000)
  {
    cameraPosition[ 2 ] += horizontalSpeed / 1000.f;
  }

  //left/right
  if (GetAsyncKeyState( 'D' ) & 0x8000)
  {
    cameraPosition[ 0 ] -= horizontalSpeed / 1000.f;
  }
  if (GetAsyncKeyState( 'A' ) & 0x8000)
  {
    cameraPosition[ 0 ] += horizontalSpeed / 1000.f;
  }

  //up/down
  if (GetAsyncKeyState( 'E' ) & 0x8000)
  {
    cameraPosition[ 1 ] += verticalSpeed / 1000.f;
  }
  if (GetAsyncKeyState( 'Q' ) & 0x8000)
  {
    cameraPosition[ 1 ] -= verticalSpeed / 1000.f;
  }
}

int main( void )
{
  ///////////////////////////////////////////////////////////////////
  // INITIALIZATION /////////////////////////////////////////////////
  //width/height of the window
  unsigned width = 1366, height = 768;

  //create the window
  HWND windowHandle = InitWindow( width, height );

  //set up graphics configuration
  rey_oso::GfxConfig config;
  config.Fullscreen_ = false;
  config.HandleToWindow_ = &windowHandle;
  config.ModelListPath_ = "../Models/";
  config.ShaderListPath_ = "../SHADER_BINARY/";
  config.TextureListPath_ = "../Textures/";
  config.WindowWidth_ = width;
  config.WindowHeight_ = height;
  config.m_renderUI = true;

  //create graphics interface
  rey_oso::GfxAPI *gfxInterface = rey_oso::GfxAPI::CreateGfxInterface( config );

  ///////////////////////////////////////////////////////////////////
  // INITALIZING VIEWPORT
  //create a camera
  GFXHND camera = gfxInterface->CameraMgr.AddCamera( );

  //setting position, looking at a point
  gfxInterface->CameraMgr.GetCamera( camera ).SetPosition( DirectX::XMFLOAT4( 0, 0, 10, 1 ) );
  gfxInterface->CameraMgr.GetCamera( camera ).LookAtPoint( DirectX::XMFLOAT4( 0, 0, 0, 1 ) );

  //create a viewport
  GFXHND viewport = gfxInterface->ViewportMgr.CreateViewport( width, height );

  //give the camera to the viewport
  gfxInterface->ViewportMgr.SetCamera( viewport, camera );
  //changing between forward and deferred rendering
  gfxInterface->ViewportMgr.SetRenderMode( viewport, ViewportRenderMode::VIEWPORT_RENDER_DEFERRED );

  ///////////////////////////////////////////////////////////////////
  // CREATING OBJECTS

  //wireframe box
  GFXHND box = gfxInterface->RenderableMgr.AddRenderable( RENDERABLE_TYPE::RENDERABLE_PRIMITIVE );

  //model
  GFXHND model = gfxInterface->RenderableMgr.AddRenderable( RENDERABLE_TYPE::RENDERABLE_MODEL3D );
  gfxInterface->RenderableMgr.GetModel3D( model ).SetModel( "Cube" );
  //SetWorldMatrix is the one probably used the most
  gfxInterface->RenderableMgr.GetModel3D( model ).SetWorldMatrix( DirectX::XMMatrixScaling( 1, 1, 1 ) );

  //floor
  GFXHND floor = gfxInterface->RenderableMgr.AddRenderable( RENDERABLE_TYPE::RENDERABLE_MODEL3D );
  gfxInterface->RenderableMgr.GetModel3D( floor ).SetModel( "Cube" );
  gfxInterface->RenderableMgr.GetModel3D( floor ).SetWorldMatrix( DirectX::XMMatrixScaling( 30, 1, 30 ) * DirectX::XMMatrixTranslation(0, -1, 0) );

  //point light
  GFXHND light = gfxInterface->RenderableMgr.AddRenderable( RENDERABLE_TYPE::RENDERABLE_POINT_LIGHT );
  gfxInterface->RenderableMgr.GetPointLight( light ).SetColor( 1, 1, 1 );
  gfxInterface->RenderableMgr.GetPointLight( light ).SetRadius( 40 );
  gfxInterface->RenderableMgr.GetPointLight( light ).SetPosition( 2, 0, 2 );

  //directional light
  GFXHND directLight = gfxInterface->RenderableMgr.AddRenderable( RENDERABLE_TYPE::RENDERABLE_DIRECTION_LIGHT );
  gfxInterface->RenderableMgr.GetDirectionalLight( directLight ).SetDirection( 0, -1, 0 );
  gfxInterface->RenderableMgr.GetDirectionalLight( directLight ).SetColor( 1, 1, 1 );

  ///////////////////////////////////////////////////////////////////
  // MAIN LOOP
  while (1)
  {
    /////////////////////////////////////////////////////////////////
    // MESSAGE UPDATE
    MSG msg;
    while (PeekMessage( &msg, NULL, NULL, 0, PM_REMOVE ))
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg ); // Matt smells
    }

    //lets the escape key close the game
    if (GetAsyncKeyState( VK_ESCAPE ) & 0x8000)
    {
      break;
    }

    /////////////////////////////////////////////////////////////////
    // MAIN UPDATE
    //some small camera stuff
    updateCamera( );
    gfxInterface->CameraMgr.GetCamera( camera ).SetPosition( DirectX::XMFLOAT4(cameraPosition[ 0 ], cameraPosition[ 1 ], cameraPosition[ 2 ], cameraPosition[ 3 ] ));

    //move the light around
    static float angle = 0;
    angle += 0.016; 
    gfxInterface->RenderableMgr.GetPointLight( light ).SetPosition( 2 * cosf(angle), 2.5, 2 * sinf( angle ) );

    //spin the cube
    gfxInterface->RenderableMgr.GetModel3D( model ).SetWorldMatrix( DirectX::XMMatrixRotationRollPitchYaw( angle , -angle , angle ) );

    /////////////////////////////////////////////////////////////////
    // GRAPHICS UPDATE
    gfxInterface->BeginScene( );
    //START OF RENDER CALLS

    gfxInterface->RenderObject( box );
    gfxInterface->RenderObject( floor );
    gfxInterface->RenderObject( model );
    gfxInterface->RenderObject( light );
    gfxInterface->RenderScene( 0.016, viewport );

    // END OF RENDER CALLS
    gfxInterface->EndScene( );
  }

  //clean up
  gfxInterface->DestroyGfxInterface( );
  return 0;
}