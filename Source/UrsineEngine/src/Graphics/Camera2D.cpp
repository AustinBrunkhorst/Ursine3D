/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Camera2D.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "Camera2D.h"

namespace Ursine
{
    void Camera2D::Update(void)
    {
        auto dt = gApplication->GetDelta();

        m_positionRaw += (m_positionTarget - m_positionRaw) * m_speedMove * dt;

        if (m_boundsConstrained)
            applyBoundConstraints();

        m_zoom += (m_zoomTarget - m_zoom) * m_speedZoom * dt;

        if (m_zoomConstrained)
            applyZoomConstraints();

        m_shakeAnimation += dt;
        m_shakeAmplitude += -m_shakeAmplitude * dt * 4.5f;

        updatePosition();

        updateMatrices();
    }

    const Vec2 &Camera2D::GetPosition(void)
    {
        return m_position;
    }

    void Camera2D::SetTargetPosition(const Vec2 &position)
    {
        m_positionTarget = position;
    }

    const Vec2 &Camera2D::GetRawPosition(void)
    {
        return m_positionRaw;
    }

    void Camera2D::SetRawPosition(const Vec2 &position)
    {
        m_positionRaw = position;

        if (m_boundsConstrained)
            applyBoundConstraints();
    }

    float Camera2D::GetZoom(void)
    {
        return m_zoom;
    }

    void Camera2D::SetZoom(float zoom)
    {
        if (m_zoomConstrained)
            m_zoom = Math::Clamp(zoom, m_constraintsZoom.X(), m_constraintsZoom.Y());
        else
            m_zoom = zoom;
    }

    void Camera2D::SetTargetZoom(float zoom)
    {
        m_zoomTarget = zoom;
    }

    const Vec2 &Camera2D::GetMoveSpeed(void)
    {
        return m_speedMove;
    }

    void Camera2D::SetMoveSpeed(const Vec2 &speed)
    {
        m_speedMove = speed;
    }

    const Vec2 &Camera2D::GetShakeSpeed(void)
    {
        return m_speedShake;
    }

    void Camera2D::SetShakeSpeed(const Vec2 &speed)
    {
        m_speedShake = speed;
    }

    float Camera2D::GetZoomSpeed(void)
    {
        return m_speedZoom;
    }

    void Camera2D::SetZoomSpeed(float speed)
    {
        m_speedZoom = speed;
    }

    void Camera2D::AddShake(float amount)
    {
        m_shakeAmplitude += amount;
    }

    void Camera2D::ClearShake(void)
    {
        m_shakeAmplitude = 0.0f;
        m_shakeAnimation = 0.0f;
    }

    const AABB &Camera2D::GetPositionConstraints(void)
    {
        return m_constraintsBounds;
    }

    void Camera2D::SetPositionConstraints(const AABB &bounds)
    {
        m_boundsConstrained = true;
        m_constraintsBounds = bounds;
    }

    void Camera2D::SetPositionConstraints(bool constrained)
    {
        m_boundsConstrained = constrained;
    }

    const Vec2 &Camera2D::GetZoomConstraints(void)
    {
        return m_constraintsZoom;
    }

    void Camera2D::SetZoomConstraints(const Vec2 &constraints)
    {
        m_zoomConstrained = true;
        m_constraintsZoom = constraints;
    }

    void Camera2D::SetZoomConstraints(bool constrained)
    {
        m_zoomConstrained = constrained;
    }

    const Mat3 &Camera2D::GetWorldToCamera(void)
    {
        return m_worldToCamera;
    }

    const Mat3 &Camera2D::GetCameraToNDC(void)
    {
        return m_cameraToNDC;
    }

    const Mat3 &Camera2D::GetWorldToNDC(void)
    {
        return m_worldToNDC;
    }

    const Mat3 &Camera2D::GetNDCToScreen(void)
    {
        return m_ndcToScreen;
    }

    Vec2 Camera2D::WorldToCamera(const Vec2 &point)
    {
        return GetWorldToCamera().TransformPoint(point);
    }

    Vec2 Camera2D::CameraToWorld(const Vec2 &point)
    {
        return Mat3::Inverse(GetWorldToCamera()).TransformPoint(point);
    }

    Vec2 Camera2D::WorldToNDC(const Vec2 &point)
    {
        return GetWorldToNDC().TransformPoint(point);
    }

    Vec2 Camera2D::NDCToWorld(const Vec2 &point)
    {
        return Mat3::Inverse(GetWorldToNDC()).TransformPoint(point);
    }

    Vec2 Camera2D::NDCToScreen(const Vec2 &point)
    {
        return m_ndcToScreen.TransformPoint(point);
    }

    Vec2 Camera2D::ScreenToNDC(const Vec2 &point)
    {
        return Mat3::Inverse(GetNDCToScreen()).TransformPoint(point);
    }

    Vec2 Camera2D::WorldToScreen(const Vec2 &point)
    {
        return NDCToScreen(WorldToNDC(point));
    }

    Vec2 Camera2D::ScreenToWorld(const Vec2 &point)
    {
        return Mat3::Inverse(GetNDCToScreen() * GetWorldToNDC()).TransformPoint(point);
    }

    void Camera2D::updatePosition(void)
    {
        float shake_pi = m_shakeAnimation * Math::PI;

        Vec2 shake
        {
            cosf(m_speedShake.X() * shake_pi) * m_shakeAmplitude,
            sinf(m_speedShake.Y() * shake_pi) * m_shakeAmplitude * VERTICAL_SCALAR
        };

        m_position = m_positionRaw + shake;
    }

    void Camera2D::applyBoundConstraints(void)
    {
        m_positionRaw.X() = Math::Clamp(m_positionRaw.X(), 
            m_constraintsBounds.low_bound.X(), m_constraintsBounds.up_bound.X());

        m_positionRaw.Y() = Math::Clamp(m_positionRaw.Y(), 
            m_constraintsBounds.low_bound.Y(), m_constraintsBounds.up_bound.Y());
    }

    void Camera2D::applyZoomConstraints(void)
    {
        m_zoom = Math::Clamp(m_zoom, m_constraintsZoom.X(), m_constraintsZoom.Y());
    }

    void Camera2D::updateMatrices(void)
    {
        m_worldToCamera.SetWorldToCamera(m_zoom, m_zoom, m_rotation, m_position);

        // update camera to ndc
        if (m_viewportSize.X() > m_viewportSize.Y())
        {
            float aspect = m_viewportSize.X() / m_viewportSize.Y();

            m_cameraToNDC.SetColumns(
                Vec3(2.0f, 0, 0),
                Vec3(0, 2.0f * aspect, 0),
                Vec3(0, 0, 1));
        }
        else
        {
            float aspect = m_viewportSize.Y() / m_viewportSize.X();

            m_cameraToNDC.SetColumns(
                Vec3(2.0f * aspect, 0, 0),
                Vec3(0, 2.0f, 0),
                Vec3(0, 0, 1));
        }

        // update world to ndc
        m_worldToNDC = m_cameraToNDC * m_worldToCamera;

        // update ndc to screen
        m_ndcToScreen.SetRows(
            Vec3(m_viewportSize.X() / 2.0f, 0, m_viewportSize.X() / 2.0f),
            Vec3(0, -m_viewportSize.Y() / 2.0f, m_viewportSize.Y() / 2.0f),
            Vec3(0, 0, 1));
    }
}