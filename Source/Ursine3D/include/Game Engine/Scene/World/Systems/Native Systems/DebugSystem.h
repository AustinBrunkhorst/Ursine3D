/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
                          const Color &color, float duration);

        private:

            graphics::GfxAPI *m_gfx;

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onUpdate(EVENT_HANDLER(World));

            // We keep track of draw requests so that we can
            // persist them for a period of time
            struct Request
            {
                float duration;
                float timer;

                Request(float duration)
                    : duration( duration )
                    , timer( 0.0f ) { }
            };

            struct LineRequest : Request
            {
                SVec3 start, end;
                Color color;

                LineRequest(const SVec3 &start, const SVec3 &end,
                            const Color &color, float duration)
                    : Request( duration )
                    , start( start )
                    , end( end )
                    , color( color ) { }
            };

            std::vector<LineRequest> m_lineRequests;

        } Meta(Enable);
    }
}
