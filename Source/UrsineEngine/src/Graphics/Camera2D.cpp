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

    const Vector2 &Camera2D::GetPosition(void)
    {
        return m_position;
    }

    void Camera2D::SetTargetPosition(const Vector2 &position)
    {
        m_positionTarget = position;
    }

    const Vector2 &Camera2D::GetRawPosition(void)
    {
        return m_positionRaw;
    }

    void Camera2D::SetRawPosition(const Vector2 &position)
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
            m_zoom = Math::Clamp(zoom, m_constraintsZoom.x, m_constraintsZoom.y);
        else
            m_zoom = zoom;
    }

    void Camera2D::SetTargetZoom(float zoom)
    {
        m_zoomTarget = zoom;
    }

    const Vector2 &Camera2D::GetMoveSpeed(void)
    {
        return m_speedMove;
    }

    void Camera2D::SetMoveSpeed(const Vector2 &speed)
    {
        m_speedMove = speed;
    }

    const Vector2 &Camera2D::GetShakeSpeed(void)
    {
        return m_speedShake;
    }

    void Camera2D::SetShakeSpeed(const Vector2 &speed)
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

    const Vector2 &Camera2D::GetZoomConstraints(void)
    {
        return m_constraintsZoom;
    }

    void Camera2D::SetZoomConstraints(const Vector2 &constraints)
    {
        m_zoomConstrained = true;
        m_constraintsZoom = constraints;
    }

    void Camera2D::SetZoomConstraints(bool constrained)
    {
        m_zoomConstrained = constrained;
    }

    const Matrix3 &Camera2D::GetWorldToCamera(void)
    {
        return m_worldToCamera;
    }

    const Matrix3 &Camera2D::GetCameraToNDC(void)
    {
        return m_cameraToNDC;
    }

    const Matrix3 &Camera2D::GetWorldToNDC(void)
    {
        return m_worldToNDC;
    }

    const Matrix3 &Camera2D::GetNDCToScreen(void)
    {
        return m_ndcToScreen;
    }

    Vector2 Camera2D::WorldToCamera(const Vector2 &point)
    {
        return GetWorldToCamera().TransformPoint(point);
    }

    Vector2 Camera2D::CameraToWorld(const Vector2 &point)
    {
        return Matrix3::Inverse(GetWorldToCamera()).TransformPoint(point);
    }

    Vector2 Camera2D::WorldToNDC(const Vector2 &point)
    {
        return GetWorldToNDC().TransformPoint(point);
    }

    Vector2 Camera2D::NDCToWorld(const Vector2 &point)
    {
        return Matrix3::Inverse(GetWorldToNDC()).TransformPoint(point);
    }

    Vector2 Camera2D::NDCToScreen(const Vector2 &point)
    {
        return m_ndcToScreen.TransformPoint(point);
    }

    Vector2 Camera2D::ScreenToNDC(const Vector2 &point)
    {
        return Matrix3::Inverse(GetNDCToScreen()).TransformPoint(point);
    }

    Vector2 Camera2D::WorldToScreen(const Vector2 &point)
    {
        return NDCToScreen(WorldToNDC(point));
    }

    Vector2 Camera2D::ScreenToWorld(const Vector2 &point)
    {
        return Matrix3::Inverse(GetNDCToScreen() * GetWorldToNDC()).TransformPoint(point);
    }

    void Camera2D::updatePosition(void)
    {
        float shake_pi = m_shakeAnimation * Math::PI;

        Vector2 shake
        {
            cosf(m_speedShake.x * shake_pi) * m_shakeAmplitude,
            sinf(m_speedShake.y * shake_pi) * m_shakeAmplitude * VERTICAL_SCALAR
        };

        m_position = m_positionRaw + shake;
    }

    void Camera2D::applyBoundConstraints(void)
    {
        m_positionRaw.x = Math::Clamp(m_positionRaw.x, 
            m_constraintsBounds.low_bound.x, m_constraintsBounds.up_bound.x);

        m_positionRaw.y = Math::Clamp(m_positionRaw.y, 
            m_constraintsBounds.low_bound.y, m_constraintsBounds.up_bound.y);
    }

    void Camera2D::applyZoomConstraints(void)
    {
        m_zoom = Math::Clamp(m_zoom, m_constraintsZoom.x, m_constraintsZoom.y);
    }

    void Camera2D::updateMatrices(void)
    {
        m_worldToCamera.SetWorldToCamera(m_zoom, m_zoom, m_rotation, m_position);

        // update camera to ndc
        if (m_viewportSize.x > m_viewportSize.y)
        {
            float aspect = m_viewportSize.x / m_viewportSize.y;

            m_cameraToNDC.SetColumns(
                Vector3(2.0f, 0, 0),
                Vector3(0, 2.0f * aspect, 0),
                Vector3(0, 0, 1));
        }
        else
        {
            float aspect = m_viewportSize.y / m_viewportSize.x;

            m_cameraToNDC.SetColumns(
                Vector3(2.0f * aspect, 0, 0),
                Vector3(0, 2.0f, 0),
                Vector3(0, 0, 1));
        }

        // update world to ndc
        m_worldToNDC = m_cameraToNDC * m_worldToCamera;

        // update ndc to screen
        m_ndcToScreen.SetRows(
            Vector3(m_viewportSize.x / 2.0f, 0, m_viewportSize.x / 2.0f),
            Vector3(0, -m_viewportSize.y / 2.0f, m_viewportSize.y / 2.0f),
            Vector3(0, 0, 1));
    }
}