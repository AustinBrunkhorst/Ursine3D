/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsManager.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "CoreSystem.h"
#include "Meta.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "PhysicsDebugDrawer.h"

namespace ursine
{
	class PhysicsManager : public core::CoreSystem
	{
		CORE_SYSTEM;
	public:

		Meta( DisableNonDynamic )
		PhysicsManager(void) { }

		Meta(Disable)
		void OnInitialize(void) override;

		Meta(Disable)
		void OnRemove(void) override;

		void AddCollisionShape(btCollisionShape *shape);

		btRigidBody *AddRigidBody(const btRigidBody::btRigidBodyConstructionInfo &info);

	private:

		// collision configuration contains default setup for memory,
		// collision setup. Advanced users can create their own configuration.
		btDefaultCollisionConfiguration m_collisionConfig;

		// use the default collision dispatcher. For parallel processing 
		// you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher *m_dispatcher;

		// btDbvtBroadphase is a good general purpose broadphase. 
		// You can also try out btAxis3Sweep.
		btBroadphaseInterface *m_overlappingPairCache;

		// the default constraint solver. For parallel processing 
		// you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver *m_solver;

		btSoftRigidDynamicsWorld *m_dynamicsWorld;

		PhysicsDebugDrawer *m_debugDrawer;

		// keep track of the shapes, we release memory at exit.
		// make sure to re-use collision shapes among rigidbodies whenever possible!
		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

		void onAppUpdate(EVENT_HANDLER(Application));

		void initPhysics(void);
		void destroyPhysics(void);

	} Meta( Enable );
}
