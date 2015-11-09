#include "UrsinePrecompiled.h"

#include "DebugSystem.h"
#include "CoreSystem.h"
#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION(DebugSystem);

        DebugSystem::DebugSystem(World *world)
            : EntitySystem( world )
            , m_gfx( GetCoreSystem( graphics::GfxAPI ) )
        {
            
        }

        void DebugSystem::DrawLine(const SVec3 &start, const SVec3 &end,
                                   const Color &color, float duration)
        {
            m_lineRequests.emplace_back( start, end, color, duration );
        }

        void DebugSystem::OnInitialize(void)
        {
            EntitySystem::OnInitialize();

            m_world->Listener(this)
                .On(WORLD_UPDATE, &DebugSystem::onUpdate);
        }

        void DebugSystem::OnRemove(void)
        {
            EntitySystem::OnRemove();

            m_world->Listener(this)
                .Off(WORLD_UPDATE, &DebugSystem::onUpdate);
        }

        void DebugSystem::onUpdate(EVENT_HANDLER(World))
        {
            float dt = Application::Instance->GetDeltaTime();
            auto &drawer = m_gfx->DrawingMgr;

            // draw all lines
            for (auto it = m_lineRequests.begin( ); it != m_lineRequests.end( );)
            {
                drawer.SetColor( it->color );
                drawer.DrawLine( it->start, it->end );

                it->timer += dt;

                if (it->timer > it->duration)
                    it = m_lineRequests.erase( it );
                else
                    ++it;
            }
        }
    }
}