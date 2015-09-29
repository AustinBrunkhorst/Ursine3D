#include "Precompiled.h"

#include "GraphicsTest.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>
#include <KeyboardManager.h>
#include <MouseManager.h>
#include <GfxAPI.h>

#include <Color.h>
#include <SQuat.h>
#include <SMat4.h>
#include <Core/Input/Keyboard/KeyboardManagerEvent.h>
#include <Core/Input/Mouse/MouseManagerEvent.h>
#include <Game Engine/Scene/Component/Core Components/TransformComponent.h>
#include <Ease.h>

using namespace ursine;

SVec3 colors[] = {
    { 1,0,0 },
    { 0,1,0 },
    { 0,0,1 },
    { 1,1,0 },
    { 1,0,1 },
    { 0,1,1 },
    { 1,1,1 },

    { 0.5,1,1 },
    { 1,0.5,1 },
    { 1,1,0.5 },
    { 0.5,0.5,1 },
    { 0.5,1,0.5 }
};

namespace
{
  //const auto kGraphicsTestUIEntryPoint = "file:///Assets/Test.html";
  const auto kGraphicsTestUIEntryPoint = "www.google.com";

  const auto kDefaultWindowWidth = 1280;
  const auto kDefaultWindowHeight = 720;

  void onResize( int width, int height )
  {
    Application::Instance->GetCoreSystem<ursine::GfxAPI>( )->Resize( width, height );
  }
}

CORE_SYSTEM_DEFINITION( GraphicsTest );

GraphicsTest::GraphicsTest( void )
  : m_mainWindow( nullptr )
{

}

GraphicsTest::~GraphicsTest( void )
{
  delete m_mainWindow;
}

void GraphicsTest::OnInitialize( void )
{
  initGraphics( );
  initPhysics( );

  m_camPos = SVec3( 0, 0, 0 );
  m_camZoom = 5;
}

void GraphicsTest::OnRemove( void )
{
  Application::Instance->Disconnect(
    APP_UPDATE,
    this,
    &GraphicsTest::onAppUpdate
    );

  m_mainWindow->Listener( this )
    .Off( WINDOW_RESIZE, &GraphicsTest::onMainWindowResize );

  m_ui->Close( );
}

void GraphicsTest::onAppUpdate( EVENT_HANDLER( Application ) )
{
  EVENT_ATTRS( Application, EventArgs );

  // @@@ TODO:
  // Update Scene
  // Render Tools
  // Render Editor UIView

  float t = 0.0f;
  t = Application::Instance->GetDeltaTime( );

  UpdateCamera( t );
  RenderGrid( );

  static SQuat quat0( 60.0f, 40.0f, 10.0f );
  static SQuat quat1( -60.0f, -40.0f, -20.0f );
  static float slerpT = 0.0f;
  static int direction = 1;

  slerpT += Application::Instance->GetDeltaTime( ) * direction;

  if (slerpT > 1.0f)
    direction = -1;
  else if (slerpT < 0.0f)
    direction = 1;

  slerpT = math::Clamp( slerpT, 0.0f, 1.0f );

  SQuat result = quat0.Slerp( quat1, ease::CircularInOut( slerpT ) );
  SMat4 mat( result );
   Model3D &modelCube = m_gfx->RenderableMgr.GetModel3D( m_cube );
  //modelCube.SetWorldMatrix( mat );

   m_gfx->RenderableMgr.GetPointLight( m_light ).SetPosition( m_camPos );
   m_gfx->RenderableMgr.GetPointLight( m_light ).SetRadius( 5 );
m_gfx->StartFrame( );
  //BEGIN

  m_gfx->BeginScene( );

  //stick draw calls here
  //m_gfx->RenderObject( m_cube );
  m_gfx->RenderObject( m_floor );
  //m_gfx->RenderObject(m_billboard);
  //m_gfx->RenderObject( m_directLight );
  //m_gfx->RenderObject( m_primitive );

  //lights
  m_gfx->RenderObject( m_light );

  static float dt;
  int count = 12;

  dt += t;

  float currAngle = 0;

  float radius = 4;
  float centerX = 4.5;
  float centerY = 4.5;

  for (int x = 0; x < count; ++x)
  {
      PointLight &current = m_gfx->RenderableMgr.GetPointLight( m_lights[ x ] );

      float cf = cosf( currAngle + dt );
      float sf = sinf( currAngle + dt );

      current.SetRadius( 3 );
      current.SetPosition( cf * radius + centerX, sf * radius + centerY, -1 );
      current.SetColor( colors[ x ].X( ), colors[ x ].Y( ), colors[ x ].Z( ) );
      currAngle += (360.f / count) * 3.14 / 180.f;

      m_gfx->RenderObject( m_lights[ x ] );
  }

    
  //boxess
  for (int y = 0; y < 10; ++y)
  {
      for (int x = 0; x < 10; ++x)
      {
          m_gfx->RenderObject( m_spheres[ y ][ x ] );
      }
  }
  
  //LAST
  m_gfx->RenderScene( 0.016f, m_camera );

  m_gfx->EndScene( );


  //END
  m_gfx->EndFrame( );

}

void GraphicsTest::onMainWindowResize( EVENT_HANDLER( ursine::Window ) )
{
  EVENT_ATTRS( ursine::Window, ursine::WindowResizeArgs );

  onResize( args->width, args->height );

  m_ui->SetViewport( {
    0, 0,
    args->width, args->height
  } );
}

void GraphicsTest::onMouseScroll( EVENT_HANDLER( MouseManager ) )
{
  EVENT_ATTRS( MouseManager, ursine::MouseScrollArgs );

  m_camZoom -= args->delta.Y( );

  if (m_camZoom < 1) m_camZoom = 1.f;
}

void GraphicsTest::UpdateCamera_Keys ( float dt )
{
    auto *app = Application::Instance;
    auto *keyboardMgr = app->GetCoreSystem<KeyboardManager>( );

  float speed = 3;

  //get the camera
  Camera &cam = m_gfx->CameraMgr.GetCamera( m_camera );
  SVec3 look = cam.GetLook( );

  ///////////////////////////////////////////////////////////////////
  // KEYBOARD MOVEMENT //////////////////////////////////////////////
  //get other camera data, now that the look was set
  auto right = cam.GetRight( );
  auto pos = m_camPos;
  auto up = cam.GetUp( );
  auto dir = SVec3( 0, 0, 0 );

  //update position
  if (keyboardMgr->IsDown( KEY_W ))
  {
    dir += look;
  }
  if (keyboardMgr->IsDown( KEY_S) )
  {
    dir -= look;
  }
  if (keyboardMgr->IsDown( KEY_A))
  {
    dir += right;
  }
  if (keyboardMgr->IsDown( KEY_D))
  {
    dir -= right;
  }

  if (keyboardMgr->IsDown( KEY_E ))
  {
    dir += up;
  }
  if (keyboardMgr->IsDown( KEY_Q ))
  {
    dir -= up;
  }

  //make sure something happened
  if (dir.Length( ) > 0)
  {
    //normalize vector, scale by dt and speed
    dir.Normalize( );
    dir *= dt * speed;

    //apply to position
    pos += dir;

    //set new pos
    m_camPos = pos;
  }
}

void GraphicsTest::UpdateCamera_Mouse ( float dt )
{
  auto *app = Application::Instance;
  auto *mouseMgr = app->GetCoreSystem<MouseManager>( );

  //get the camera
  Camera &cam = m_gfx->CameraMgr.GetCamera( m_camera );

  SVec3 look = cam.GetLook( );
  SVec3 up = cam.GetUp( );
  SVec3 right = cam.GetRight( );

  ///////////////////////////////////////////////////////////////////
  // CAMERA ROTATION
  if (mouseMgr->IsButtonDown( MBTN_LEFT ))
  {
    auto mouseDelta = mouseMgr->GetPositionDelta( );
    auto camTransform = cam.GetViewMatrix( );

    //we need to limit the up delta so that we can't wrap if we are at the very top/bottom
    mouseDelta /= 2.f;

    if (mouseDelta.Length( ) > 0)
    {
      //generate the up rotation
      auto upRotation = SQuat( -mouseDelta.Y( ), camTransform.TransformVector( SVec3( 1.0, 0.0, 0.0 ) ) );

      //generate side rotation
      auto sideRotation = SQuat( -mouseDelta.X( ), SVec3( 0, 1, 0 ) );

      //transform w/ rotations
      look = sideRotation.Rotate( look );
      look = upRotation.Rotate( look );

      cam.SetLook( look );
    }
  }

  ///////////////////////////////////////////////////////////////////
  // PANNING
  else if(mouseMgr->IsButtonDown( MBTN_MIDDLE ))
  {
    auto mouseDelta = mouseMgr->GetPositionDelta( );

    if (mouseDelta.Length( ) > 0)
    {
      m_camPos += right * -mouseDelta.X( ) * dt * kDefaultWindowWidth / 1366;

      m_camPos += up * -mouseDelta.Y( ) * dt * kDefaultWindowHeight / 768;
    }
  }

  ///////////////////////////////////////////////////////////////////
  // ZOOM
  else if (mouseMgr->IsButtonDown( MBTN_RIGHT ))
  {
    auto mouseDelta = mouseMgr->GetPositionDelta( );

    if (mouseDelta.Length( ) > 0)
    {
      m_camZoom += -mouseDelta.Y( ) * dt;

      if(m_camZoom < 1)
      {
        m_camZoom = 1;
      }
    }
  }

  //if in here, draw the center mark
  float halfSize = 0.3f;

  m_gfx->DrawingMgr.SetColor( 1, 0, 0, 1 );
  m_gfx->DrawingMgr.DrawLine( m_camPos - SVec3( halfSize, 0, 0 ), m_camPos + SVec3( halfSize, 0, 0 ) );
  m_gfx->DrawingMgr.DrawPoint( m_camPos + SVec3( halfSize, 0, 0 ) );
  m_gfx->DrawingMgr.SetColor( 0, 1, 0, 1 );
  m_gfx->DrawingMgr.DrawLine( m_camPos - SVec3( 0, halfSize, 0 ), m_camPos + SVec3( 0, halfSize, 0 ) );
  m_gfx->DrawingMgr.DrawPoint( m_camPos + SVec3( 0, halfSize, 0 ) );
  m_gfx->DrawingMgr.SetColor( 0, 0, 1, 1 );
  m_gfx->DrawingMgr.DrawLine( m_camPos - SVec3( 0, 0, halfSize ), m_camPos + SVec3( 0, 0, halfSize ) );
  m_gfx->DrawingMgr.DrawPoint( m_camPos + SVec3( 0, 0, halfSize ) );
}

void GraphicsTest::UpdateCamera( float dt )
{
  auto *app = Application::Instance;
  auto *keyboardMgr = app->GetCoreSystem<KeyboardManager>( );

  // first, update all of the camera stuff. 
  //keyboard controls
  UpdateCamera_Keys( dt );

  // mouse controls
  if (keyboardMgr->GetModifiers() & KMD_ALT)
  {
    UpdateCamera_Mouse( dt );
  }

  //our position always needs to be relative to the center position
  Camera &cam = m_gfx->CameraMgr.GetCamera( m_camera );
  SVec3 look = cam.GetLook( );

  //normalize look and scale by zoom
  look.Normalize( );
  look = look * m_camZoom;

  //negate the vector, opposite of look is going away from center
  cam.SetPosition( m_camPos - look );

  ///////////////////////////////////////////////////////////////////
  //render the xyz axis in side window
  Camera &cam2 = m_gfx->CameraMgr.GetCamera( m_camera2 );
  float lineLength = 2;

  look.Normalize( );
  cam2.SetLook( look );
  cam2.SetPosition( SVec3( 0, 0, 1000 ) - look * 5 );
  
  SVec3 pointPos = SVec3( 0, 0, 1000);
  m_gfx->DrawingMgr.SetColor( 1, 0, 0, 1 ); //x
  m_gfx->DrawingMgr.DrawLine( pointPos, pointPos - SVec3( lineLength, 0, 0 ));
  m_gfx->DrawingMgr.SetColor( 0, 1, 0, 1 ); //y
  m_gfx->DrawingMgr.DrawLine( pointPos, pointPos + SVec3( 0, lineLength, 0 ) );
  m_gfx->DrawingMgr.SetColor( 0, 0, 1, 1 ); //z
  m_gfx->DrawingMgr.DrawLine( pointPos, pointPos + SVec3( 0, 0, lineLength * 2 ) );
}

void GraphicsTest::RenderGrid ( )
{
  ///////////////////////////////////////////////////////////////////
  // VARIABLES
  //rendering a grid
  float gridColor[ 4 ] = { 0.3, 0.3, 0.3, 1};
  float xColor[ 4 ] = { 0.58, 0.07, 0.07, 1 };
  float zColor[ 4 ] = { 0, 0.12, 0.75, 1};
  float subColor[ 4 ] = { 0, 0, 0, 1 };

  //size of a cell
  float cellSize = 1;

  // # of cells
  int widthCount = 80;
  int heightCount = 80;

  //number of cells between dividers
  static int subSector = 10;

  //current center position of the grid
  float centerX = m_camPos.X( );
  float centerZ = m_camPos.Z( );

  ///////////////////////////////////////////////////////////////////
  // IMPLEMENTATION
  //make sure it is off
  if (widthCount % 2 == 0)
    widthCount++;
  if (heightCount % 2 == 0)
    heightCount++;

  float scalar = (1.f / cellSize);
  if (scalar < 1) scalar = 1;

  //round to the right size
  float posX = round( centerX * (1.f / cellSize) ) / (1.f / cellSize);
  float posZ = round( centerZ * (1.f / cellSize) ) / (1.f / cellSize);

  //to get to the side, get the middle
  posX -= (int)(widthCount / 2) * cellSize;

  //render w/ X changing
  for (int x = 0; x < widthCount; ++x)
  {
    if (round( posX * scalar ) / scalar == 0)
      m_gfx->DrawingMgr.SetColor( zColor[0], zColor[ 1 ], zColor[ 2 ], zColor[ 3 ] );
    else if ((int)(posX * (int)scalar) % subSector == 0)
      m_gfx->DrawingMgr.SetColor( subColor[ 0 ], subColor[ 1 ], subColor[ 2 ], subColor[ 3 ] );
    else
      m_gfx->DrawingMgr.SetColor( gridColor[ 0 ], gridColor[ 1 ], gridColor[ 2 ], gridColor[ 3 ] );

    m_gfx->DrawingMgr.DrawLine( posX, 0, posZ + heightCount / 2 * cellSize, posX, 0, posZ - heightCount / 2 * cellSize );
    posX += cellSize;
  }

  posX = round( centerX * (1.f / cellSize) ) / (1.f / cellSize);
  posZ -= (int)(heightCount / 2) * cellSize;

  //render w/ z changing
  for (int x = 0; x < heightCount; ++x)
  {
    if (round( posZ * scalar ) / scalar == 0)
      m_gfx->DrawingMgr.SetColor( xColor[ 0 ], xColor[ 1 ], xColor[ 2 ], xColor[ 3 ] );
    else if ((int)(posZ * (int)scalar) % subSector == 0)
      m_gfx->DrawingMgr.SetColor( subColor[ 0 ], subColor[ 1 ], subColor[ 2 ], subColor[ 3 ] );
    else
      m_gfx->DrawingMgr.SetColor( gridColor[ 0 ], gridColor[ 1 ], gridColor[ 2 ], gridColor[ 3 ] );

    m_gfx->DrawingMgr.DrawLine( posX + (widthCount / 2) * cellSize, 0, posZ, posX - (widthCount / 2) * cellSize, 0, posZ );
    posZ += cellSize;
  }
}

void GraphicsTest::initGraphics( void )
{
  auto *app = Application::Instance;

  app->Connect(
    APP_UPDATE,
    this,
    &GraphicsTest::onAppUpdate
    );

  auto *keyboardManager = app->GetCoreSystem<KeyboardManager>( );
  auto *mouseManager = app->GetCoreSystem<MouseManager>( );

  auto *windowManager = app->GetCoreSystem<WindowManager>( );
  auto *uiManager = app->GetCoreSystem<UIManager>( );

  m_mainWindow = windowManager->AddWindow(
    "Ursine3D Editor",
    { 0, 0 },
    { static_cast<float>(kDefaultWindowWidth), static_cast<float>(kDefaultWindowHeight) },
    SDL_WINDOW_RESIZABLE
    );

  m_mainWindow->Listener( this )
    .On( WINDOW_RESIZE, &GraphicsTest::onMainWindowResize );

  m_mainWindow->SetLocationCentered( );
  m_mainWindow->Show( true );

  //connect to keyboard/mouse events
  mouseManager->Listener( this ).On( MM_SCROLL, &GraphicsTest::onMouseScroll );

  /////////////////////////////////////////////////////////////////
  //make graphics manager
  m_gfx = app->GetCoreSystem<GfxAPI>( );

  HWND handle = reinterpret_cast<HWND>((m_mainWindow->GetPlatformHandle( )));

  GfxConfig config;
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

  m_gfx->StartGraphics( config );

  m_ui = uiManager->CreateView( m_mainWindow, kGraphicsTestUIEntryPoint );

  m_ui->SetViewport( {
    0, 0,
    kDefaultWindowWidth, kDefaultWindowHeight
  } );

  onResize( kDefaultWindowWidth, kDefaultWindowHeight );


  ////////////////////////////////////////////////////////////////////
  //initialize the demo related tings
  m_viewport = m_gfx->ViewportMgr.CreateViewport( kDefaultWindowWidth/2, kDefaultWindowHeight/2 );
  m_viewport2 = m_gfx->ViewportMgr.CreateViewport( 200, 200 );

  m_gfx->SetGameViewport( m_viewport );

  m_gfx->ViewportMgr.GetViewport( m_viewport2 ).SetPosition( kDefaultWindowWidth - 250, kDefaultWindowHeight - 250 );
  
  m_gfx->ViewportMgr.GetViewport( m_viewport ).SetRenderMode( VIEWPORT_RENDER_DEFERRED );
  m_gfx->ViewportMgr.GetViewport( m_viewport2 ).SetRenderMode( VIEWPORT_RENDER_FORWARD );
  m_gfx->ViewportMgr.GetViewport( m_viewport2 ).SetBackgroundColor( 0, 0, 0, 0 );
  
  m_camera = m_gfx->CameraMgr.AddCamera( );
  m_camera2 = m_gfx->CameraMgr.AddCamera( );
  m_gfx->CameraMgr.GetCamera( m_camera ).LookAtPoint( SVec3( 0, 0, 0 ) );
  m_gfx->CameraMgr.GetCamera( m_camera2 ).SetPosition( SVec3( 0, 0, 1010 ) );
  m_gfx->CameraMgr.GetCamera( m_camera2 ).SetProjMode( Camera::PROJECTION_ORTHOGRAPHIC );
  m_gfx->CameraMgr.GetCamera( m_camera2 ).SetPlanes( 0.0000, 100 );

  m_gfx->CameraMgr.GetCamera( m_camera ).SetPosition( 0, 0 );
  m_gfx->CameraMgr.GetCamera( m_camera ).SetDimensions( 1, 1 );

  m_cube = m_gfx->RenderableMgr.AddRenderable( RENDERABLE_MODEL3D );
  m_floor = m_gfx->RenderableMgr.AddRenderable( RENDERABLE_MODEL3D );
  m_primitive = m_gfx->RenderableMgr.AddRenderable(RENDERABLE_PRIMITIVE);
  m_billboard = m_gfx->RenderableMgr.AddRenderable( RENDERABLE_BILLBOARD2D );
  m_directLight = m_gfx->RenderableMgr.AddRenderable( RENDERABLE_DIRECTION_LIGHT );

  m_gfx->RenderableMgr.GetDirectionalLight( m_directLight ).SetDirection( 0, -1, 0 );

  m_light = m_gfx->RenderableMgr.AddRenderable( RENDERABLE_POINT_LIGHT );
  m_light2 = m_gfx->RenderableMgr.AddRenderable( RENDERABLE_POINT_LIGHT );
  m_gfx->ViewportMgr.GetViewport( m_viewport ).SetViewportCamera( m_camera );
  m_gfx->ViewportMgr.GetViewport( m_viewport2 ).SetViewportCamera( m_camera2 );

  Model3D &MdlCube = m_gfx->RenderableMgr.GetModel3D( m_cube );
  PointLight &pointLight = m_gfx->RenderableMgr.GetPointLight( m_light );
  PointLight &pointLight2 = m_gfx->RenderableMgr.GetPointLight( m_light2 );
  Billboard2D &billboard = m_gfx->RenderableMgr.GetBillboard2D( m_billboard );
  // Primitive &primitive = gfx->RenderableMgr.GetPrimitive(wirePrimitive);

  int count = 10;

  for (int y = 0; y < count; ++y)
  {
      for (int x = 0; x < count; ++x)
      {
          m_spheres[ y ][ x ] = m_gfx->RenderableMgr.AddRenderable( RENDERABLE_MODEL3D );

          Model3D &current = m_gfx->RenderableMgr.GetModel3D( m_spheres[ y ][ x ] );
          current.SetWorldMatrix( SMat4( SVec3(x, y, 0 ) ) );
          current.SetModel( "Cube" );
          current.SetMaterial( "Blank" );
          current.SetMaterialData( 0.5, 10.f + (x / (float)(count - 1)) * 245.f, (float)y / (float)(count - 1) );
      }
  }

  for (int x = 0; x < 12; ++x)
  {
      m_lights[x] = m_gfx->RenderableMgr.AddRenderable( RENDERABLE_POINT_LIGHT );
  }

  //set primitive data
  // primitive.SetRadius(3.0f);
  // primitive.SetType(Primitive::PRIM_SPHERE);	//what type of shape do you want? sphere? capsule? cube? plane?

  SMat4 trans;
  trans.Translate( SVec3( 0, -8, 0 ) );
  m_gfx->RenderableMgr.GetModel3D( m_floor ).SetWorldMatrix( trans * SMat4( 10, 10, 10 ) );

  trans.Translate( SVec3( 1, 0, 0 ) );
  billboard.SetWorldMatrix( trans );

  //set obj data here
  MdlCube.SetModel( "Character" );
  MdlCube.SetMaterial( "Cube" );
  MdlCube.SetWorldMatrix( SMat4::Identity( ) );

  pointLight.SetPosition( 0, 2, -2 );
  pointLight.SetRadius( 40 );

  pointLight2.SetPosition( 0, 2, 2 );
  pointLight2.SetRadius( 40 );
}

void GraphicsTest::initPhysics( void )
{
  /*int i;

  m_physics = Application::Instance->GetCoreSystem<PhysicsManager>();

  ///create a few basic rigid bodies
  btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

  //keep track of the shapes, we release memory at exit.
  //make sure to re-use collision shapes among rigid bodies whenever possible!
  btAlignedObjectArray<btCollisionShape*> collisionShapes;

  collisionShapes.push_back(groundShape);

  btTransform groundTransform;
  groundTransform.setIdentity();
  groundTransform.setOrigin(btVector3(0, -56, 0));

  {
  btScalar mass(0.);

  //rigidbody is dynamic if and only if mass is non zero, otherwise static
  bool isDynamic = (mass != 0.f);

  btVector3 localInertia(0, 0, 0);
  if (isDynamic)
  groundShape->calculateLocalInertia(mass, localInertia);

  //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
  btRigidBody* body = new btRigidBody(rbInfo);

  //add the body to the dynamics world
  dynamicsWorld->addRigidBody(body);
  }


  {
  //create a dynamic rigidbody

  //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
  btCollisionShape* colShape = new btSphereShape(btScalar(1.));
  collisionShapes.push_back(colShape);

  /// Create Dynamic Objects
  btTransform startTransform;
  startTransform.setIdentity();

  btScalar	mass(1.f);

  //rigidbody is dynamic if and only if mass is non zero, otherwise static
  bool isDynamic = (mass != 0.f);

  btVector3 localInertia(0, 0, 0);
  if (isDynamic)
  colShape->calculateLocalInertia(mass, localInertia);

  startTransform.setOrigin(btVector3(2, 10, 0));

  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
  btRigidBody* body = new btRigidBody(rbInfo);

  dynamicsWorld->addRigidBody(body);
  }



  /// Do some simulation


  ///-----stepsimulation_start-----
  for (i = 0; i<100; i++)
  {
  dynamicsWorld->stepSimulation(1.f / 60.f, 10);

  //print positions of all objects
  for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
  {
  btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
  btRigidBody* body = btRigidBody::upcast(obj);
  btTransform trans;
  if (body && body->getMotionState())
  {
  body->getMotionState()->getWorldTransform(trans);

  }
  else
  {
  trans = obj->getWorldTransform();
  }
  printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
  }
  }

  ///-----stepsimulation_end-----

  //cleanup in the reverse order of creation/initialization

  ///-----cleanup_start-----

  //remove the rigidbodies from the dynamics world and delete them
  for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
  {
  btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
  btRigidBody* body = btRigidBody::upcast(obj);
  if (body && body->getMotionState())
  {
  delete body->getMotionState();
  }
  dynamicsWorld->removeCollisionObject(obj);
  delete obj;
  }

  //delete collision shapes
  for (int j = 0; j<collisionShapes.size(); j++)
  {
  btCollisionShape* shape = collisionShapes[j];
  collisionShapes[j] = 0;
  delete shape;
  }



  //next line is optional: it will be cleared by the destructor when the array goes out of scope
  collisionShapes.clear();*/
}