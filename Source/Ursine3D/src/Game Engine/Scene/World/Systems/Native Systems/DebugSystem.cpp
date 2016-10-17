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

        void DebugSystem::DrawSphereFaded(const SVec3 &center, float radius, const SVec3 &cameraLook, 
                                          const SVec3 &cameraPos, const Color &color, float duration, 
                                          bool overdraw)
        {
            DrawCircleFaded(center, SVec3::UnitX(), radius, cameraLook, cameraPos, color, duration, overdraw);
            DrawCircleFaded(center, SVec3::UnitY(), radius, cameraLook, cameraPos, color, duration, overdraw);
            DrawCircleFaded(center, SVec3::UnitZ(), radius, cameraLook, cameraPos, color, duration, overdraw);
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

            SVec3 basePoint = endPoint - direction * length * 0.3f;

            // main line
            DrawLine( start, endPoint, color, duration, overdraw );

            // circle
            DrawCircle( basePoint, direction, length * 0.07f, color, duration, overdraw );
            
            // offset lines
            DrawLine( endPoint, basePoint + u * length * 0.07f, color, duration, overdraw );
            DrawLine( endPoint, basePoint - u * length * 0.07f, color, duration, overdraw );
            DrawLine( endPoint, basePoint + v * length * 0.07f, color, duration, overdraw );
            DrawLine( endPoint, basePoint - v * length * 0.07f, color, duration, overdraw );
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
            auto unitY = normDir.X( ) == 0.0f && normDir.Z( ) == 0.0f;
            auto perpDir = SVec3::Cross( normDir, unitY ? SVec3::UnitX( ) : SVec3::UnitY( ) );

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

        void DebugSystem::DrawCircleFaded(const SVec3 &center, const SVec3 &normal, float radius,
                                          const SVec3 &cameraLook, const SVec3 &cameraPos, const Color &color, 
                                          float duration, bool overdraw)
        {
            static const int resolution = 80;

            auto normDir = SVec3::Normalize( normal );
            auto unitY = normDir.X( ) == 0.0f && normDir.Z( ) == 0.0f;
            auto perpDir = SVec3::Cross( normDir, unitY ? SVec3::UnitX( ) : SVec3::UnitY( ) );

            perpDir.Normalize( );

            float theta = 0.0f;
            float step = 360.0f / resolution;

            for ( int i = 0; i < resolution; ++i )
            {
                SQuat q1( theta, normDir );

                theta += step;

                SQuat q2( theta, normDir );

                auto p1 = center + q1.Rotate( perpDir ) * radius;
                auto p2 = center + q2.Rotate( perpDir ) * radius;

                auto camToPoint = p2 - center;
                float dot = camToPoint.Dot( cameraLook );

                if(dot < 0)
                    DrawLine( p1, p2, color, duration, overdraw );
                else
                    DrawLine( p1, p2, Color( color.r * 0.2f, color.g * 0.2f, color.b * 0.2f, color.a * 0.2f ), duration, overdraw );
            }
        }

        void DebugSystem::DrawCone(const SVec3 &tipPosition, SVec3 focusPosition,
                                   float focalLength, float focalAngleDegrees, 
                                   const Color &color, float duration, bool overdraw)
        {
            // The direction from the base to the focal point
            auto dir = focusPosition - tipPosition;

            // The orthogonal vectors
            SVec3 u, v;

            dir.GenerateOrthogonalVectors( u, v );

            // The radius of the focal ring
            float radius = focalLength * sinf( math::DegreesToRadians( focalAngleDegrees ) );

            if (focalAngleDegrees > 45.0f)
            {
                float angle = focalAngleDegrees - 45.0f;
                float t = angle / 45.0f;

                focusPosition -= dir * t;
            }

            // render the circle
            DrawCircle( focusPosition, dir, radius, color, duration, overdraw );

            // render the lines
            DrawLine( tipPosition, focusPosition + u * radius, color, duration, overdraw );
            DrawLine( tipPosition, focusPosition - u * radius, color, duration, overdraw );
            DrawLine( tipPosition, focusPosition + v * radius, color, duration, overdraw );
            DrawLine( tipPosition, focusPosition - v * radius, color, duration, overdraw );
        }

        void DebugSystem::OnInitialize(void)
        {
            EntitySystem::OnInitialize( );

            m_world->Listener(this)
            #if defined(URSINE_WITH_EDITOR)
                .On( WORLD_EDITOR_UPDATE, &DebugSystem::onUpdate )
            #endif
                .On( WORLD_UPDATE, &DebugSystem::onWorldUpdate );
        }

        void DebugSystem::OnRemove(void)
        {
            EntitySystem::OnRemove();

            m_world->Listener(this)
            #if defined(URSINE_WITH_EDITOR)
                .Off( WORLD_EDITOR_UPDATE, &DebugSystem::onUpdate )
            #endif
                .Off( WORLD_UPDATE, &DebugSystem::onWorldUpdate );
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

        void DebugSystem::onWorldUpdate(EVENT_HANDLER(World))
        {
            m_requests.clear( );
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