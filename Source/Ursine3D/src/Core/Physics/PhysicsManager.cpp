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
/*#include "PhysicsManager.h"

namespace ursine
{
	CORE_SYSTEM_DEFINITION( PhysicsManager );

	 PhysicsManager::PhysicsManager(void)
	 {
		m_dispatcher = new btCollisionDispatcher( &m_collisionConfig );
		m_overlappingPairCache = new btDbvtBroadphase( );
		m_solver = new btSequentialImpulseConstraintSolver( );

		m_dynamicsWorld = new btDiscreteDynamicsWorld(
					          m_dispatcher, 
					          m_overlappingPairCache, 
					          m_solver,
					          &m_collisionConfig
						  );

		m_dynamicsWorld->setGravity( btVector3( 0, -10, 0 ) );

		// Debug drawing
		m_debugDrawer = new PhysicsDebugDrawer(Application::Instance->GetCoreSystem<GfxAPI>());
		m_dynamicsWorld->setDebugDrawer(m_debugDrawer);
		m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	 }

	 PhysicsManager::~PhysicsManager(void)
	 {
		 //delete dynamics world
		 delete m_dynamicsWorld;

		 //delete solver
		 delete m_solver;

		 //delete broadphase
		 delete m_overlappingPairCache;

		 //delete dispatcher
		 delete m_dispatcher;
	 }

}
*/