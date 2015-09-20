/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsDebugDrawer.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "LinearMath/btIDebugDraw.h"

namespace ursine
{
	class GfxAPI;

	class PhysicsDebugDrawer : public btIDebugDraw
	{
		GfxAPI *m_gfx;
		int m_debugMode;

	public:
		PhysicsDebugDrawer(GfxAPI *gfx);

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, 
							  btScalar distance, int lifeTime, const btVector3& color)  override;

		void reportErrorWarning(const char* warningString) override;

		void draw3dText(const btVector3& location, const char* textString) override;

		void setDebugMode(int debugMode) override;

		int	getDebugMode() const override;
	};
}
