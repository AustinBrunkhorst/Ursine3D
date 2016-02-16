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
                                   const Color &color, float duration, 
                                    bool overdraw)
        {
            m_requests.emplace_back( 
                std::make_shared<LineRequest>( start, end, color, duration, overdraw)
            );
        }

        void DebugSystem::DrawPoint(const SVec3 &point, float size, 
                                    const Color &color, float duration,
                                    bool overdraw)
        {
            m_requests.emplace_back( 
                std::make_shared<PointRequest>(point, size, color, duration, overdraw )
            );
        }

        void DebugSystem::DrawSphere(const SVec3& center, float radius, 
                                     const Color& color, float duration,
                                     bool overdraw)
        {
            DrawCircle(center, SVec3::UnitX( ), radius, color, duration, overdraw );
            DrawCircle(center, SVec3::UnitY( ), radius, color, duration, overdraw );
            DrawCircle(center, SVec3::UnitZ( ), radius, color, duration, overdraw );
        }

        void DebugSystem::DrawRay(const SVec3& start, const SVec3& direction, 
                                  float length, const Color& color, float duration, 
                                  bool overdraw)
        {
            SVec3 endPoint = start + direction * length;
            SVec3 u, v;
            direction.GenerateOrthogonalVectors( u, v );

            u.Normalize( );
            v.Normalize( );

            SVec3 basePoint = endPoint - direction * length * 0.25f;

            // main line
            DrawLine( start, endPoint, color, duration, overdraw );

            // circle
            DrawCircle( basePoint, direction, length * 0.125f, color, duration, overdraw );
            
            // offset lines
            DrawLine( endPoint, basePoint + u * length * 0.125f, color, duration, overdraw );
            DrawLine( endPoint, basePoint - u * length * 0.125f, color, duration, overdraw );
            DrawLine( endPoint, basePoint + v * length * 0.125f, color, duration, overdraw );
            DrawLine( endPoint, basePoint - v * length * 0.125f, color, duration, overdraw );
        }

        void DebugSystem::DrawCube(const SVec3& center, float size, 
                                   const Color& color, float duration, 
                                   bool overdraw)
        {
            float halfSize = size;
            // generate the 8 corners
            auto ntl = (center + SVec3(-halfSize,  halfSize, -halfSize));
            auto ntr = (center + SVec3(halfSize,  halfSize, -halfSize));
            auto nbl = (center + SVec3(-halfSize, -halfSize, -halfSize));
            auto nbr = (center + SVec3(halfSize, -halfSize, -halfSize));
                       
            auto ftl = (center + SVec3(-halfSize, halfSize, halfSize));
            auto ftr = (center + SVec3(halfSize, halfSize, halfSize));
            auto fbl = (center + SVec3(-halfSize, -halfSize, halfSize));
            auto fbr = (center + SVec3(halfSize, -halfSize, halfSize));

            // near square
            DrawLine(ntl, ntr, color, duration, overdraw); // top
            DrawLine(ntr, nbr, color, duration, overdraw); // right
            DrawLine(nbr, nbl, color, duration, overdraw); // bottom
            DrawLine(nbl, ntl, color, duration, overdraw); // left

            // far square
            DrawLine(ftl, ftr, color, duration, overdraw); // top
            DrawLine(ftr, fbr, color, duration, overdraw); // right
            DrawLine(fbr, fbl, color, duration, overdraw); // bottom
            DrawLine(fbl, ftl, color, duration, overdraw); // left

            // connecting lines
            DrawLine(ntl, ftl, color, duration, overdraw);
            DrawLine(ntr, ftr, color, duration, overdraw);
            DrawLine(nbl, fbl, color, duration, overdraw);
            DrawLine(nbr, fbr, color, duration, overdraw);
        }

        void DebugSystem::DrawCircle(const SVec3& center, const SVec3& normal, float radius, 
                                     const Color& color, float duration,
                                     bool overdraw)
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

                DrawLine( p1, p2, color, duration, overdraw );
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
            draw.SetOverdraw(overdraw);
            draw.SetColor( color );
            draw.DrawLine( start, end );
            draw.SetOverdraw(false);
        }

        void DebugSystem::PointRequest::Draw(graphics::DrawingAPI &draw)
        {
            draw.SetOverdraw(overdraw);
            draw.SetColor( color );
            draw.SetSize( size );
            draw.DrawPoint( point );
            draw.SetOverdraw(false);
        }
    }
}