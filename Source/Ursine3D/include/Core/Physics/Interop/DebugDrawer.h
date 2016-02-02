/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DebugDrawer.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "PhysicsInteropConfig.h"
#include "GfxAPI.h"

namespace ursine
{
    namespace physics
    {
        enum DrawMode
        {
		#ifdef BULLET_PHYSICS
            
			DRAW_NONE = btIDebugDraw::DBG_NoDebug,
            DRAW_WIRE_FRAME = btIDebugDraw::DBG_DrawWireframe,
            DRAW_AABB = btIDebugDraw::DBG_DrawAabb,
            DRAW_CONTACT_POINTS = btIDebugDraw::DBG_DrawContactPoints

		#endif
        };

        class DebugDrawer : public DebugDrawerBase
        {
        public:
            DebugDrawer(graphics::GfxAPI *gfx);

#ifdef BULLET_PHYSICS
            void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

            void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
                                  btScalar distance, int lifeTime, const btVector3& color)  override;

            void reportErrorWarning(const char* warningString) override;

            void draw3dText(const btVector3& location, const char* textString) override;
#endif
            void setDebugMode(int debugMode) override;

            void appendDebugMode(DrawMode drawMode);

            void removeDebugMode(DrawMode drawMode);

            int	getDebugMode(void) const override;

        private:
            graphics::GfxAPI *m_gfx;
            int m_debugMode;
        };
    }
}
