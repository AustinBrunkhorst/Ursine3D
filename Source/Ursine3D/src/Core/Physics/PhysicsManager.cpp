/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsManager.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "PhysicsManager.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"

namespace ursine
{
	CORE_SYSTEM_DEFINITION( PhysicsManager );

	void PhysicsManager::OnInitialize()
	{
		initPhysics();

		auto *app = Application::Instance;

		app->Connect( APP_UPDATE, this, &PhysicsManager::onAppUpdate );
	}

	void PhysicsManager::OnRemove()
	{
		destroyPhysics();
	}

	void PhysicsManager::AddCollisionShape(btCollisionShape* shape)
	{
		m_collisionShapes.push_back(shape);
	}

	btRigidBody *PhysicsManager::AddRigidBody(const btRigidBody::btRigidBodyConstructionInfo& info)
	{
		auto body = new btRigidBody(info);

		//add the body to the dynamics world
		m_dynamicsWorld->addRigidBody(body);

		return body;
	}

    void PhysicsManager::LoadWorld(const char* bulletFile)
    {
        auto *fileLoader = new btBulletWorldImporter(m_dynamicsWorld);
        UAssert(fileLoader->loadFile(bulletFile), "Failed to load Bullet World");
        delete fileLoader;
    }

    void PhysicsManager::onAppUpdate(EVENT_HANDLER(Application))
	{
		m_dynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
		m_dynamicsWorld->debugDrawWorld();
	}

	void PhysicsManager::initPhysics()
	{
		m_dispatcher = new btCollisionDispatcher(&m_collisionConfig);
		m_overlappingPairCache = new btDbvtBroadphase();
		m_solver = new btSequentialImpulseConstraintSolver();

		m_dynamicsWorld = new btSoftRigidDynamicsWorld(
			m_dispatcher,
			m_overlappingPairCache,
			m_solver,
			&m_collisionConfig
		);

		m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

		// Debug drawing
		m_debugDrawer = new PhysicsDebugDrawer( GetCoreSystem(graphics::GfxAPI ) );
		m_dynamicsWorld->setDebugDrawer(m_debugDrawer);
		m_dynamicsWorld->getDebugDrawer()->setDebugMode(
            btIDebugDraw::DBG_DrawWireframe | 
            btIDebugDraw::DBG_DrawContactPoints
        );
	}

	void PhysicsManager::destroyPhysics()
	{
		//remove the rigidbodies from the dynamics world and delete them
		for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}

			m_dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}

		//delete collision shapes
		for (int i = 0; i < m_collisionShapes.size(); i++)
		{
			btCollisionShape* shape = m_collisionShapes[i];
			m_collisionShapes[i] = nullptr;
			delete shape;
		}

		//delete dynamics world
		delete m_dynamicsWorld;

		//delete solver
		delete m_solver;

		//delete broadphase
		delete m_overlappingPairCache;

		//delete dispatcher
		delete m_dispatcher;

		m_collisionShapes.clear();
	}
}
