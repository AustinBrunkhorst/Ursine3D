/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Camera.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "Camera.h"
#include "DirectXMath.h"

using namespace ursine;

namespace ursine
{
    namespace graphics
    {
        void Camera::Initialize(void)
        {
            m_position.Set( 0, 0, -10 );
            LookAtPoint( SVec3( 0, 0, 0 ) );

            m_projMode = PROJECTION_PERSPECTIVE;
            m_fov = 45.f;

            m_nearPlane = 0.1f;
            m_farPlane = 100.f;
            m_size = 10.f;

            m_renderMode = VIEWPORT_RENDER_DEFERRED;

            m_cameraMask = 0;

            m_clearColor = Color( 0.f, 0.f, 0.f, 1.f );
        }

        void Camera::Uninitialize(void) { }

        SMat4 Camera::GetViewMatrix(void) const
        {
            auto eye = SVec4( m_position, 1.0f ).ToD3D( );
            auto target = SVec4( m_position + m_look, 1 ).ToD3D( );
            auto up = SVec4( m_up, 0 ).ToD3D( );

            auto eyePos = XMLoadFloat4( &eye );
            auto targetPos = XMLoadFloat4( &target );
            auto upDir = XMLoadFloat4( &up );

            return SMat4( DirectX::XMMatrixLookAtLH( eyePos, targetPos, upDir ) );
        }

        SMat4 Camera::GetProjMatrix(void) const
        {
            if (m_projMode == PROJECTION_PERSPECTIVE)
            {
                return SMat4( DirectX::XMMatrixPerspectiveFovLH( m_fov * 3.14f / 180.f,
                                                                 m_screenWidth / m_screenHeight,
                                                                 m_nearPlane, m_farPlane ) );
            }
            else
            {
                return SMat4( DirectX::XMMatrixOrthographicLH( m_size *
                                                               (m_screenWidth / m_screenHeight),
                                                               m_size, m_nearPlane, m_farPlane ) );
            }
        }

        const SVec3 &Camera::GetPosition(void) const
        {
            return m_position;
        }

        void Camera::SetPosition(const SVec3 &pos)
        {
            m_position = pos;
        }

        const SVec3 &Camera::GetLook(void) const
        {
            return m_look;
        }

        void Camera::SetLook(const SVec3 &look)
        {
            m_look = look;
            CalculateVectors( SVec3::UnitY( ) );
        }

        const SVec3 &Camera::GetRight(void) const
        {
            return m_right;
        }

        const SVec3 &Camera::GetUp(void) const
        {
            return m_up;
        }

        void Camera::SetPlanes(const float nearPlane, const float farPlane)
        {
            UAssert( nearPlane < farPlane, 
                "Near and far planes cannot be the same!"
            );

            m_nearPlane = nearPlane;
            m_farPlane = farPlane;
        }

        void Camera::GetPlanes(float &nearPlane, float &farPlane) const
        {
            nearPlane = m_nearPlane;
            farPlane = m_farPlane;
        }

        float Camera::GetFOV(void) const
        {
            return m_fov;
        }

        void Camera::SetFOV(const float fov)
        {
            UAssert( fov > 0, 
                "FOV can't be less than 0!"
            );

            m_fov = fov;
        }

        Camera::ProjectionMode Camera::GetProjMode(void) const
        {
            return m_projMode;
        }

        void Camera::SetProjMode(const ProjectionMode mode)
        {
            m_projMode = mode;
        }

        float Camera::GetSize(void)
        {
            return m_size;
        }

        void Camera::SetSize(const float size)
        {
            m_size = size;
        }

        void Camera::LookAtPoint(const SVec3 &point)
        {
            m_look = point - m_position;
            CalculateVectors( SVec3::UnitY( ) );
        }

        void Camera::SetDimensions(const float width, const float height)
        {
            m_width = width;
            m_height = height;
        }

        void Camera::GetDimensions(float &width, float &height) const
        {
            width = m_width;
            height = m_height;
        }

        void Camera::SetViewportPosition(const float x, const float y)
        {
            m_xPos = x;
            m_yPos = y;
        }

        void Camera::GetViewportPosition(float &x, float &y) const
        {
            x = m_xPos;
            y = m_yPos;
        }

        void Camera::SetRenderMode(const ViewportRenderMode renderMode)
        {
            m_renderMode = renderMode;
        }

        ViewportRenderMode Camera::GetRenderMode(void) const
        {
            return m_renderMode;
        }

        SVec3 Camera::ScreenToWorld(const Vec2 &screenPos, const float depth)
        {
            UAssert(depth >= 0, "Can't use a number less than 0 for depth!");

            //convert mouse coordinates to NDC. Also offset for the screen position
            float unitX = 2.f * ((screenPos.X( ) - m_screenX) / m_screenWidth) - 1;
            float unitY = -(2.f * ((screenPos.Y( ) - m_screenY) / m_screenHeight)) + 1;

            ////create point with depth
            //Vec4 point = Vec4(unitX, unitY, depth, 1);

            SMat4 viewMat = GetViewMatrix( );
            SMat4 projMat = GetProjMatrix( );
            viewMat.Transpose( );
            projMat.Transpose( );

            SMat4 invViewProj = projMat * viewMat;
            invViewProj.Inverse( );

            Vec4 pos = invViewProj * Vec4( unitX, unitY, depth, 1 );
            pos /= pos.W( );

            return Vec3( pos.X( ), pos.Y( ), pos.Z( ) );

            //////get inv projection
            //SMat4 invProj = GetProjMatrix();
            //invProj.Transpose( );               //transpose because we use DX projection matrix
            //SMat4::Inverse(invProj);

            //Vec4 pos = invProj * Vec4(unitX, unitY, depth, 1);
            //pos /= pos.W();

            //printf( "view space pos: %f, %f, %f\n", pos.X( ), pos.Y( ), pos.Z( ) );

            ////get inv view
            //SMat4 invView = GetViewMatrix();
            //invView.Transpose( );               //transpose because we use DX view matrix
            //SMat4::Inverse(invView);

            //pos = invView * pos;

            ////return
            //Vec3 finalPoint;
            //finalPoint.Set(point.X(), point.Y(), point.Z());
            //return finalPoint;
            return SVec3( pos.X( ), pos.Y( ), pos.Z( ) );
        }

        void Camera::SetScreenDimensions(const float width, const float height)
        {
            m_screenWidth = width;
            m_screenHeight = height;
        }

        void Camera::SetScreenPosition(const float x, const float y)
        {
            m_screenX = x;
            m_screenY = y;
        }

        bool Camera::CheckMask(const unsigned long long renderMask)
        {
            // I know the bitshift is crazy, it's handled, don't worry
        #pragma warning( push )
        #pragma warning( disable : 4293)

            // check to see if the whitelist bit is set
            if (renderMask & (0x1u << 63u))
                return (renderMask - (0x1u << 63u)) == m_entity->GetID( );

            // else, return the regular mask comparison
            return (renderMask & m_cameraMask) != 0;

        #pragma warning( pop ) 
        }

        unsigned Camera::GetMask(void) const
        {
            return static_cast<unsigned>( m_cameraMask );
        }

        void Camera::SetMask(const unsigned long long renderMask)
        {
            m_cameraMask = renderMask;
        }

        const ecs::EntityHandle &Camera::GetEntity(void) const
        {
            return m_entity;
        }

        void Camera::SetEntity(const ecs::EntityHandle id)
        {
            m_entity = id;
        }

        void Camera::CalculateVectors(const SVec3 &up)
        {
            m_look.Normalize( );

            m_right = SVec3::Cross( m_look, up );
            m_right.Normalize( );

            m_up = SVec3::Cross( -m_look, m_right );
            m_up.Normalize( );
        }

        const Color &Camera::GetClearColor(void) const
        {
            return m_clearColor;
        }

        void Camera::SetClearColor(const Color &color)
        {
            m_clearColor = color;
        }
    }
}
