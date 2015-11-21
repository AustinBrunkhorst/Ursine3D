/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
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
            m_position.Set(0, 0, -10);
            LookAtPoint(SVec3(0, 0, 0));

            m_projMode = PROJECTION_PERSPECTIVE;
            m_fov = 45.f;

            m_nearPlane = 0.1f;
            m_farPlane = 100.f;
            m_size = 10.f;

            m_renderMode = VIEWPORT_RENDER_DEFERRED;
        }

        void Camera::Uninitialize(void)
        {

        }

        SMat4 Camera::GetViewMatrix(void) const
        {
            auto eye = SVec4(m_position, 1.0f).ToD3D();
            auto target = SVec4(m_position + m_look, 1).ToD3D();
            auto up = SVec4(m_up, 0).ToD3D();

            auto eyePos = XMLoadFloat4(&eye);
            auto targetPos = XMLoadFloat4(&target);
            auto upDir = XMLoadFloat4(&up);

            return SMat4(DirectX::XMMatrixLookAtLH(eyePos, targetPos, upDir));
        }

        SMat4 Camera::GetProjMatrix() const
        {
            SMat4 mat;

            if (m_projMode == PROJECTION_PERSPECTIVE)
            {
                return SMat4(DirectX::XMMatrixPerspectiveFovLH(m_fov * 3.14f / 180.f, 
                    m_screenWidth / m_screenHeight, 
                    m_nearPlane, m_farPlane));
            }
            else
            {
                return SMat4(DirectX::XMMatrixOrthographicLH(m_size * 
                    (m_screenWidth / m_screenHeight), 
                    m_size, m_nearPlane, m_farPlane));
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
            CalculateVectors(SVec3::UnitY());
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
            UAssert(nearPlane < farPlane, "Near and far planes cannot be the same!");

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
            UAssert(fov > 0, "FOV can't be less than 0!");
            
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

        const float Camera::GetSize()
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
            CalculateVectors(SVec3::UnitY());
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

        SVec3 Camera::ScreenToWorld(const Vec2& screenPos, const float depth)
        {
            UAssert(depth > 0, "Can't use a number less than 0 for depth!");

            //convert mouse coordinates to NDC. Also offset for the screen position
            float unitX = 2.f * ((screenPos.X() - m_screenX) / m_screenWidth) - 1;
            float unitY = -(2.f * ((screenPos.Y() - m_screenY) / m_screenHeight)) + 1;

            ////create point with depth
            //Vec4 point = Vec4(unitX, unitY, depth, 1);

            ////get inv projection
            SMat4 invProj = GetProjMatrix();
            SMat4::Transpose(invProj);
            SMat4::Inverse(invProj);

            Vec4 pos = invProj * Vec4(unitX, unitY, depth, 1);
            pos /= pos.W();

            //get inv view
            SMat4 invView = GetViewMatrix();
            SMat4::Transpose(invView);
            SMat4::Inverse(invView);

            SVec4 temp = invView * SVec4(0, 0, 0, 1);

            pos = invView * pos;

            ////apply inv proj
            //point = invProj * point;

            ////divide by w
            //point /= point.W();

            ////apply inv view
            //point = invView * point;

            //

            ////return
            //Vec3 finalPoint;
            //finalPoint.Set(point.X(), point.Y(), point.Z());
            //return finalPoint;
            return SVec3(pos.X(), pos.Y(), pos.Z());
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

        void Camera::CalculateVectors(const SVec3 &up)
        {
            m_look.Normalize();

            m_right = SVec3::Cross(m_look, up);
            m_right.Normalize();

            m_up = SVec3::Cross(-m_look, m_right);
            m_up.Normalize();
        }
    }
}