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
            m_requests.emplace_back( 
                std::make_shared<LineRequest>( start, end, color, duration )
            );
        }

        void DebugSystem::DrawPoint(const SVec3 &point, float size, 
                                    const Color &color, float duration)
        {
            m_requests.emplace_back( 
                std::make_shared<PointRequest>(point, size, color, duration )
            );
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
            float dt = Application::Instance->GetDeltaTime( );

            // draw all requests
            for (auto it = m_requests.begin( ); it != m_requests.end( );)
            {
                (*it)->Draw( m_gfx->DrawingMgr );

                (*it)->timer += dt;

                if ((*it)->timer > (*it)->duration)
                    it = m_requests.erase( it );
                else
                    ++it;
            }
        }

        void DebugSystem::LineRequest::Draw(graphics::DrawingAPI &draw)
        {
            draw.SetColor( color );
            draw.DrawLine( start, end );
        }

        void DebugSystem::PointRequest::Draw(graphics::DrawingAPI &draw)
        {
            draw.SetColor( color );
            draw.SetSize( size );
            draw.DrawPoint( point );
        }
    }
}