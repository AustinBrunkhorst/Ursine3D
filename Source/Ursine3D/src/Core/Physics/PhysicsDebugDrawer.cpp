/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsDebugDrawer.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
/*
#include "PhysicsDebugDrawer.h"
#include "GfxAPI.h"

namespace ursine
{
	PhysicsDebugDrawer::PhysicsDebugDrawer(GfxAPI* gfx)
		: m_gfx(gfx) { }

	void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		m_gfx->DrawingMgr.SetColor(color.getX(), color.getY(), color.getZ(), 1.0f);

		m_gfx->DrawingMgr.DrawLine(
			from.getX(), from.getY(), from.getZ(),
			to.getX(), to.getY(), to.getZ()
		);
	}

	void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, 
										      btScalar distance, int lifeTime, const btVector3& color)
	{
		m_gfx->DrawingMgr.SetColor(color.getX(), color.getY(), color.getZ(), 1.0f);

		m_gfx->DrawingMgr.DrawPoint(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());

		auto normP = PointOnB + normalOnB * distance;

		m_gfx->DrawingMgr.DrawLine(
			PointOnB.getX(), PointOnB.getY(), PointOnB.getZ(),
			normP.getX(), normP.getY(), normP.getZ()
		);
	}

	void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
	{
		// Generate an error warning
	}

	void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
	{
		// draw some text
	}

	void PhysicsDebugDrawer::setDebugMode(int debugMode)
	{
		m_debugMode = debugMode;
	}

	int PhysicsDebugDrawer::getDebugMode() const
	{
		return m_debugMode;
	}
}
*/