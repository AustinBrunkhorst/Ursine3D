#include "Precompiled.h"

#include "PhysicsTest.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>
#include <GfxAPI.h>

#include <Color.h>
#include <SQuat.h>
#include <SMat4.h>
#include <Game Engine/Scene/Component/Native Components/CameraComponent.h>

using namespace ursine;

namespace
{
    //const auto kGraphicsTestUIEntryPoint = "file:///Assets/Test.html";
    const auto kGraphicsTestUIEntryPoint = "www.google.com";

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;

    void onResize(int width, int height)
	{
        GetCoreSystem( ursine::GfxAPI )->Resize( width, height );
    }
}

CORE_SYSTEM_DEFINITION( PhysicsTest );

PhysicsTest::PhysicsTest(void)
    : m_mainWindow( nullptr )
{
    
}

PhysicsTest::~PhysicsTest(void)
{
    delete m_mainWindow;
}

void PhysicsTest::OnInitialize(void)
{
	initGraphics();
	initPhysics();
}

void PhysicsTest::OnRemove(void)
{
    Application::Instance->Disconnect(
        APP_UPDATE, 
        this, 
        &PhysicsTest::onAppUpdate 
    );

    m_mainWindow->Listener( this )
        .Off( WINDOW_RESIZE, &PhysicsTest::onMainWindowResize );

    m_ui->Close( );
}

void PhysicsTest::onAppUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);

    // @@@ TODO:
    // Update Scene
    // Render Tools
    // Render Editor UIView

	static float t = 0.0f;
	t += Application::Instance->GetDeltaTime();

    auto &cam = m_gfx->CameraMgr.GetCamera(m_camera);

	float distance = 11.0f;
	float x = cos(t) * distance;
	float z = sin(t) * distance;
	cam.SetPosition( SVec3( x, 0.0f, z ) );

	SVec3 dir( cam.GetPosition( ) );
	cam.SetLook( -dir );

	static SQuat quat0(60.0f, 40.0f, 10.0f);
	static SQuat quat1(-60.0f, -40.0f, -20.0f);
	static float slerpT = 0.0f;
	static int direction = 1;

	slerpT += Application::Instance->GetDeltaTime() * direction;

	if (slerpT > 1.0f)
		direction = -1;
	else if (slerpT < 0.0f)
		direction = 1;

	slerpT = math::Clamp( slerpT, 0.0f, 1.0f );

	SQuat result = quat0.Slerp(quat1, ease::CircularInOut(slerpT));
	SMat4 mat(result);

	Model3D &modelCube = m_gfx->RenderableMgr.GetModel3D(m_cube);
	modelCube.SetWorldMatrix( mat );
	

	m_gfx->BeginScene( );
	//stick draw calls here
	m_gfx->RenderObject(m_cube);
	m_gfx->RenderObject(m_floor);
	// gfx->RenderObject(wirePrimitive);
	m_gfx->RenderObject(m_light);
	m_gfx->RenderObject(m_light2);

	//LAST
	m_gfx->RenderScene(0.016f, m_viewport);
	m_gfx->EndScene( );

    // m_ui->Draw();
}

void PhysicsTest::onMainWindowResize(EVENT_HANDLER(ursine::Window))
{
    EVENT_ATTRS(ursine::Window, ursine::WindowResizeArgs);

    onResize( args->width, args->height );

    m_ui->SetViewport( {
        0, 0,
        args->width, args->height
    } );
}

void PhysicsTest::initGraphics(void)
{
	auto *app = Application::Instance;

	app->Connect(
		APP_UPDATE,
		this,
		&PhysicsTest::onAppUpdate
		);

	auto *windowManager = GetCoreSystem( WindowManager );
	auto *uiManager = GetCoreSystem( UIManager );

	m_mainWindow = windowManager->AddWindow(
		"Ursine3D Editor",
		{ 0, 0 },
		{ static_cast<float>(kDefaultWindowWidth), static_cast<float>(kDefaultWindowHeight) },
		SDL_WINDOW_RESIZABLE
		);

	m_mainWindow->Listener(this)
		.On(WINDOW_RESIZE, &PhysicsTest::onMainWindowResize);

	m_mainWindow->SetLocationCentered();
	m_mainWindow->Show(true);

	/////////////////////////////////////////////////////////////////
	//make graphics manager
	m_gfx = GetCoreSystem( GfxAPI );

	HWND handle = reinterpret_cast<HWND>((m_mainWindow->GetPlatformHandle()));

	GfxConfig config;
	config.Fullscreen_ = false;
	config.HandleToWindow_ = handle;
	config.ModelListPath_ = "Models/";
	config.ShaderListPath_ = "SHADER_BINARY/";
	config.TextureListPath_ = "Textures/";
	config.WindowWidth_ = kDefaultWindowWidth;
	config.WindowHeight_ = kDefaultWindowHeight;
	config.m_renderUI = false;
	config.Profile_ = false;

	m_gfx->StartGraphics(config);

	m_ui = uiManager->CreateView(m_mainWindow, kGraphicsTestUIEntryPoint);

	m_ui->SetViewport({
		0, 0,
		kDefaultWindowWidth, kDefaultWindowHeight
	});

	onResize(kDefaultWindowWidth, kDefaultWindowHeight);


	////////////////////////////////////////////////////////////////////
	//initialize the demo related tings
	m_viewport = m_gfx->ViewportMgr.CreateViewport(kDefaultWindowWidth, kDefaultWindowHeight);
	// m_gfx->ViewportMgr.GetViewport(m_viewport).SetRenderMode(VIEWPORT_RENDER_FORWARD);
	m_camera = m_gfx->CameraMgr.AddCamera();

	m_cube = m_gfx->RenderableMgr.AddRenderable(RENDERABLE_MODEL3D);
	m_floor = m_gfx->RenderableMgr.AddRenderable(RENDERABLE_MODEL3D);
	// wirePrimitive = gfx->RenderableMgr.AddRenderable(RENDERABLE_PRIMITIVE);

	m_light = m_gfx->RenderableMgr.AddRenderable(RENDERABLE_POINT_LIGHT);
	m_light2 = m_gfx->RenderableMgr.AddRenderable(RENDERABLE_POINT_LIGHT);
	// m_gfx->ViewportMgr.GetViewport(m_viewport).SetViewportCamera(m_camera);
	m_gfx->ViewportMgr.GetViewport(m_viewport).SetDimensions(kDefaultWindowWidth, kDefaultWindowHeight);

	Model3D &MdlCube = m_gfx->RenderableMgr.GetModel3D(m_cube);
	PointLight &pointLight = m_gfx->RenderableMgr.GetPointLight(m_light);
	PointLight &pointLight2 = m_gfx->RenderableMgr.GetPointLight(m_light2);
	// Primitive &primitive = gfx->RenderableMgr.GetPrimitive(wirePrimitive);

	//set primitive data
	// primitive.SetRadius(3.0f);
	// primitive.SetType(Primitive::PRIM_SPHERE);	//what type of shape do you want? sphere? capsule? cube? plane?

	SMat4 trans;
	trans.Translate( SVec3( 0, -8, 0 ) );
	m_gfx->RenderableMgr.GetModel3D(m_floor).SetWorldMatrix(trans * SMat4( 10, 10, 10 ));

	//set obj data here
	MdlCube.SetModel("Cube");
	MdlCube.SetMaterial("Cube");
	MdlCube.SetWorldMatrix( SMat4::Identity( ) );

	pointLight.SetPosition(0, 2, -2);
	pointLight.SetRadius(40);

	pointLight2.SetPosition(0, 2, 2);
	pointLight2.SetRadius(40);
}

void PhysicsTest::initPhysics(void)
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