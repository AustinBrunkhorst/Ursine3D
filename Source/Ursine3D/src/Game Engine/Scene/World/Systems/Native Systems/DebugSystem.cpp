/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DebugSystem.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

        void DebugSystem::DrawSphere(const SVec3& center, float radius, 
                                     const Color& color, float duration)
        {
            DrawCircle(center, SVec3::UnitX( ), radius, color, duration );
            DrawCircle(center, SVec3::UnitY( ), radius, color, duration );
            DrawCircle(center, SVec3::UnitZ( ), radius, color, duration );
        }

        void DebugSystem::DrawCircle(const SVec3& center, const SVec3& normal, float radius, 
                                     const Color& color, float duration)
        {
            static const int resolution = 30;

            auto normDir = SVec3::Normalize( normal );
            auto perpDir = SVec3::Cross( normDir, normDir == SVec3::UnitY( ) ? SVec3::UnitX( ) : SVec3::UnitY( ) );

            perpDir.Normalize( );

            float theta = 0.0f;
            float step = 360.0f / resolution;

            for (int i = 0; i < resolution; ++i)
            {
                SQuat q1( theta, normDir );

                theta += step;

                SQuat q2( theta, normDir );

                auto p1 = center + q1.Rotate( perpDir ) * radius;
                auto p2 = center + q2.Rotate( perpDir ) * radius;

                DrawLine( p1, p2, color, duration );
            }
        }

        void DebugSystem::OnInitialize(void)
        {
            EntitySystem::OnInitialize( );

            m_world->Listener(this)
                .On(WORLD_EDITOR_UPDATE, &DebugSystem::onUpdate);
        }

        void DebugSystem::OnRemove(void)
        {
            EntitySystem::OnRemove();

            m_world->Listener(this)
                .Off(WORLD_EDITOR_UPDATE, &DebugSystem::onUpdate);
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