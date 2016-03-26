/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsSystem.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EntitySystem.h"
#include "SVec3.h"
#include "Color.h"

namespace ursine
{
    namespace graphics
    {
        class DrawingAPI;
        class GfxAPI;
    }

    namespace ecs
    {
        class DebugSystem : public EntitySystem
        {
            ENTITY_SYSTEM;

        public:
            DebugSystem(World *world);

            void DrawLine(const SVec3 &start, const SVec3 &end, 
                          const Color &color, float duration, bool overdraw = false);

            void DrawPoint(const SVec3 &point, float size, 
                           const Color &color, float duration, bool overdraw = false);

            void DrawSphere(const SVec3 &center, float radius,
                            const Color &color, float duration, bool overdraw = false);

            void DrawSphereFaded(const SVec3 &center, float radius, const SVec3 &cameraLook,
                                 const SVec3 &cameraPos, const Color &color, float duration, 
                                 bool overdraw = false);

            void DrawCircle(const SVec3 &center, const SVec3 &normal, float radius,
                            const Color &color, float duration, bool overdraw = false);

            void DrawCircleFaded(const SVec3 &center, const SVec3 &normal, float radius,
                                 const SVec3 &cameraLook, const SVec3 &cameraPos, const Color &color, 
                                 float duration, bool overdraw = false);

            void DrawRay(const SVec3 &start, const SVec3 &direction, float length, 
                         const Color &color,float duration, bool overdraw = false);

            void DrawCube(const SVec3 &center, float size,
                          const Color &color, float duration, bool overdraw = false);

            void DrawCone(const SVec3 &tipPosition, SVec3 focusPosition,
                          float focalLength, float focalAngleDegrees);

        private:

            graphics::GfxAPI *m_gfx;

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onUpdate(EVENT_HANDLER(World));
            void onWorldUpdate(EVENT_HANDLER(World));

            // We keep track of draw requests so that we can
            // persist them for a period of time
            struct Request
            {
                float duration;
                float timer;
                bool overdraw;

                virtual void Draw(graphics::DrawingAPI &drawer) = 0;

                Request(float duration, bool overdraw)
                    : duration( duration )
                    , timer( 0.0f ) 
                    , overdraw(overdraw) { }
            };

            struct LineRequest : Request
            {
                SVec3 start, end;
                Color color;

                LineRequest(const SVec3 &start, const SVec3 &end,
                            const Color &color, float duration, 
                            bool useOverdraw)
                    : Request( duration, useOverdraw)
                    , start( start )
                    , end( end )
                    , color( color ) { }

                void Draw(graphics::DrawingAPI &draw) override;
            };

            struct PointRequest : Request
            {
                SVec3 point;
                float size;
                Color color;

                PointRequest(const SVec3 point, float size,
                             const Color &color, float duration, 
                             bool useOverdraw)
                    : Request( duration, useOverdraw)
                    , point( point )
                    , size( size )
                    , color( color ) { }

                void Draw(graphics::DrawingAPI &draw) override;
            };

            std::vector< std::shared_ptr<Request> > m_requests;

        } Meta(Enable, AutoAddEntitySystem);
    }
}
