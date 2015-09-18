#include "Precompiled.h"

#include "PhysicsTest.h"

#include <Application.h>

#include <WindowManager.h>
#include <UIManager.h>
#include <GfxAPI.h>

#include <Color.h>
#include <SQuat.h>
#include <SMat4.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

using namespace ursine;

namespace
{
    //const auto kGraphicsTestUIEntryPoint = "file:///Assets/Test.html";
    const auto kGraphicsTestUIEntryPoint = "www.google.com";

    const auto kDefaultWindowWidth = 1280;
    const auto kDefaultWindowHeight = 720;

    void onResize(int width, int height)
    {
        // ::OPENGL
        /*glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );

        glViewport( 0, 0, width, height );
        glOrtho( 0, width, 0, height, -100.0f, 100.0f );*/

        //ursine::Application::Instance->GetCoreSystem<ursine::GfxAPI>( )->Resize( width, height );
    }
}

CORE_SYSTEM_DEFINITION( PhysicsTest );

PhysicsTest::PhysicsTest(void)
    : m_mainWindow( nullptr )
{
    
}

PhysicsTest::~PhysicsTest(void)
{
    // ::OPENGL
    //SDL_GL_DeleteContext( m_glContext );

    delete m_mainWindow;
}

void PhysicsTest::OnInitialize(void)
{
    auto *app = Application::Instance;

    app->Connect(
        APP_UPDATE, 
        this, 
        &PhysicsTest::onAppUpdate 
    );

    auto *windowManager = app->GetCoreSystem<WindowManager>( );
    auto *uiManager = app->GetCoreSystem<UIManager>( );

    m_mainWindow = windowManager->AddWindow(
        "Ursine3D Editor", 
        { 0, 0 }, 
        { static_cast<float>( kDefaultWindowWidth ), static_cast<float>( kDefaultWindowHeight ) },
        SDL_WINDOW_RESIZABLE
    );

    m_mainWindow->Listener( this )
        .On( WINDOW_RESIZE, &PhysicsTest::onMainWindowResize );
  
    m_mainWindow->SetLocationCentered( );
    m_mainWindow->Show( true );

    /////////////////////////////////////////////////////////////////
    //make graphics manager
    auto *gfxManager = app->GetCoreSystem<GfxAPI>( );

    HWND handle = reinterpret_cast<HWND>((m_mainWindow->GetPlatformHandle( )));

    GfxConfig config;
    config.Fullscreen_ = false;
    config.HandleToWindow_ = &handle;
    config.ModelListPath_ = "Models/";
    config.ShaderListPath_ = "SHADER_BINARY/";
    config.TextureListPath_ = "Textures/";
    config.WindowWidth_ = kDefaultWindowWidth;
    config.WindowHeight_ = kDefaultWindowHeight;
    config.m_renderUI = true;
    config.Profile_ = false;

    gfxManager->StartGraphics( config );

    /* m_ui = uiManager->CreateView( m_mainWindow, kGraphicsTestUIEntryPoint );

    m_ui->SetViewport( {
        0, 0,
        kDefaultWindowWidth, kDefaultWindowHeight
    } );

    onResize( kDefaultWindowWidth, kDefaultWindowHeight );*/
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

    // m_ui->Close( );
}

void PhysicsTest::onAppUpdate(EVENT_HANDLER(Application))
{
    EVENT_ATTRS(Application, EventArgs);

    // @@@ TODO:
    // Update Scene
    // Render Tools
    // Render Editor UIView

    auto gfx = Application::Instance->GetCoreSystem<GfxAPI>( );

	static GFXHND viewport;
	static GFXHND camera;
	static GFXHND cube;
	// static GFXHND wirePrimitive;
	static GFXHND floor;
	static GFXHND light;
	static bool run = false;
	
	if (!run)
	{
		run = true;

		viewport = gfx->ViewportMgr.CreateViewport(kDefaultWindowWidth, kDefaultWindowHeight);
		camera = gfx->CameraMgr.AddCamera();

		cube = gfx->RenderableMgr.AddRenderable(RENDERABLE_MODEL3D);
		floor = gfx->RenderableMgr.AddRenderable(RENDERABLE_MODEL3D);
		// wirePrimitive = gfx->RenderableMgr.AddRenderable(RENDERABLE_PRIMITIVE);

		light = gfx->RenderableMgr.AddRenderable(RENDERABLE_POINT_LIGHT);
		gfx->ViewportMgr.GetViewport(viewport).SetViewportCamera(camera);
		gfx->ViewportMgr.GetViewport(viewport).SetDimensions(kDefaultWindowWidth, kDefaultWindowHeight);

		Model3D &MdlCube = gfx->RenderableMgr.GetModel3D(cube);
		PointLight &pointLight = gfx->RenderableMgr.GetPointLight(light);
		// Primitive &primitive = gfx->RenderableMgr.GetPrimitive(wirePrimitive);

		//set primitive data
		// primitive.SetRadius(3.0f);
		// primitive.SetType(Primitive::PRIM_SPHERE);	//what type of shape do you want? sphere? capsule? cube? plane?

		gfx->RenderableMgr.GetModel3D(floor).SetWorldMatrix(DirectX::XMMatrixScaling(10, 1, 10) * DirectX::XMMatrixTranslation(0, -2, 0));

		//set obj data here
		MdlCube.SetModel("Cube");
		MdlCube.SetMaterial("Cube");
		MdlCube.SetWorldMatrix(DirectX::XMMatrixIdentity());

		pointLight.SetPosition(0, 2, -2);
		pointLight.SetRadius(40);




		///-----includes_end-----

		int i;
		///-----initialization_start-----

		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

		btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

		dynamicsWorld->setGravity(btVector3(0, -10, 0));

		///-----initialization_end-----

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

		//delete dynamics world
		delete dynamicsWorld;

		//delete solver
		delete solver;

		//delete broadphase
		delete overlappingPairCache;

		//delete dispatcher
		delete dispatcher;

		delete collisionConfiguration;

		//next line is optional: it will be cleared by the destructor when the array goes out of scope
		collisionShapes.clear();

		///-----cleanup_end-----
		printf("Press a key to exit\n");





	}

	static float t = 0.0f;
	t += Application::Instance->GetDeltaTime();

	Camera &cam = gfx->CameraMgr.GetCamera(camera);

	float distance = 5.0f;
	float x = cos(t) * distance;
	float z = sin(t) * distance;
	cam.SetPosition( DirectX::XMFLOAT4(x, 0.0f, z, 1.0f) );

	SVec4 dir(cam.GetPosition());
	cam.SetLook( (-dir).ToDxVec() );

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

	Model3D &modelCube = gfx->RenderableMgr.GetModel3D(cube);
	modelCube.SetWorldMatrix(DirectX::XMMATRIX(mat.GetFloatPtr()));
	

    gfx->BeginScene( );
	//stick draw calls here
	gfx->RenderObject(cube);
	gfx->RenderObject(floor);
	// gfx->RenderObject(wirePrimitive);
	gfx->RenderObject(light);

	//LAST
	gfx->RenderScene(0.016f, viewport);
    gfx->EndScene( );

    // m_ui->Draw( );
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