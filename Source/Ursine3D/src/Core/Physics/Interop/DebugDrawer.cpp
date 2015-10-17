#include "UrsinePrecompiled.h"

#include "DebugDrawer.h"

namespace ursine
{
    namespace physics
    {
        DebugDrawer::DebugDrawer(graphics::GfxAPI* gfx)
            : m_gfx( gfx )
            , m_debugMode( DRAW_NONE ) { }

#ifdef BULLET_PHYSICS
        void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
        {
            m_gfx->DrawingMgr.SetColor(color.getX(), color.getY(), color.getZ(), 1.0f);

		    m_gfx->DrawingMgr.DrawLine(
			    from.getX(), from.getY(), from.getZ(),
			    to.getX(), to.getY(), to.getZ()
		    );
        }

        void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, 
                                           btScalar distance, int lifeTime, const btVector3& color)
        {
            m_gfx->DrawingMgr.SetColor(color.getX(), color.getY(), color.getZ(), 1.0f);
            m_gfx->DrawingMgr.SetSize(8.0f);
		    m_gfx->DrawingMgr.DrawPoint(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
            m_gfx->DrawingMgr.SetSize(4.0f);

		    auto normP = PointOnB + normalOnB * distance;

		    m_gfx->DrawingMgr.DrawLine(
			    PointOnB.getX(), PointOnB.getY(), PointOnB.getZ(),
			    normP.getX(), normP.getY(), normP.getZ()
		    );
        }

        void DebugDrawer::reportErrorWarning(const char* warningString)
        {
            // Generate an error warning
            UWarning(warningString);
        }

        void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
        {
            // draw some text
        }
#endif
        void DebugDrawer::setDebugMode(int debugMode)
        {
            m_debugMode = debugMode;
        }

        void DebugDrawer::appendDebugMode(DrawMode drawMode)
        {
            utils::FlagSet(m_debugMode, drawMode);
        }

        void DebugDrawer::removeDebugMode(DrawMode drawMode)
        {
            utils::FlagUnset(m_debugMode, drawMode);
        }

        int DebugDrawer::getDebugMode(void) const
        {
            return m_debugMode;
        }
    }
}
