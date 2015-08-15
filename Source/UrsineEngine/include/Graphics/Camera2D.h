/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Camera2D.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
** -------------------------------------------------------------------------*/

#pragma once

#include "WindowManager.h"

#include "Vector2.h"

#include "AABB.h"

namespace Ursine
{
    class Camera2D
    {
        // arbitrary value that's applied to vertical shake, otherwise
        // it looks really odd
        const float VERTICAL_SCALAR = 0.7f;

        // position with applied shake
        Vector2 m_position = Vector2::Zero();

        // position without applied shake
        Vector2 m_positionRaw = Vector2::Zero();

        // target position that we're moving to
        Vector2 m_positionTarget = Vector2::Zero();

        // window size of the application
        const Vector2 &m_viewportSize = gWindowManager->GetSize();

        // current zoom level (relative to the size)
        float m_zoom = 1.0f;

        // target zoom level
        float m_zoomTarget = 1.0f;

        // rotation of the camera
        float m_rotation = 0.0f;

        // speed in which the camera moves to the target
        Vector2 m_speedMove = Vector2::Zero();

        // speed in which the camera shakes
        Vector2 m_speedShake = Vector2::Zero();

        // speed in which the camera zooms to it's target
        float m_speedZoom = 0.0f;

        float m_shakeAmplitude = 0.0f;
        float m_shakeAnimation = 0.0f;

        // determines if the camera's position is constrained
        bool m_boundsConstrained = false;

        // determines if the camera's zoom is constrained
        bool m_zoomConstrained = false;

        // if constrained, the bounds for the camera
        AABB m_constraintsBounds;

        // if constrained, the zoom bounds (min, max)
        Vector2 m_constraintsZoom = Vector2::Up();

        // World to camera matrix
        Matrix3 m_worldToCamera;

        // Camera to NDC matrix
        Matrix3 m_cameraToNDC;

        // World to NDC matrix
        Matrix3 m_worldToNDC;

        // NDC to screen matrix
        Matrix3 m_ndcToScreen;

        void updatePosition(void);

        void applyBoundConstraints(void);
        void applyZoomConstraints(void);

        void updateMatrices(void);

    public:
        Camera2D(void) = default;

        void Update(void);

        const Vector2 &GetPosition(void);

        void SetTargetPosition(const Vector2 &position);

        const Vector2 &GetRawPosition(void);
        void SetRawPosition(const Vector2 &position);

        float GetZoom(void);
        void SetZoom(float zoom);
        void SetTargetZoom(float zoom);

        const Vector2 &GetMoveSpeed(void);
        void SetMoveSpeed(const Vector2 &speed);

        const Vector2 &GetShakeSpeed(void);
        void SetShakeSpeed(const Vector2 &speed);

        float GetZoomSpeed(void);
        void SetZoomSpeed(float zoom);

        void AddShake(float amount);
        void ClearShake(void);

        const AABB &GetPositionConstraints(void);
        void SetPositionConstraints(const AABB &bounds);
        void SetPositionConstraints(bool constrained);

        const Vector2 &GetZoomConstraints(void);
        void SetZoomConstraints(const Vector2 &constraints);
        void SetZoomConstraints(bool constrained);

        const Matrix3 &GetWorldToCamera(void);
        const Matrix3 &GetCameraToNDC(void);
        const Matrix3 &GetWorldToNDC(void);
        const Matrix3 &GetNDCToScreen(void);

        Vector2 WorldToCamera(const Vector2 &point);
        Vector2 CameraToWorld(const Vector2 &point);
        Vector2 WorldToNDC(const Vector2 &point);
        Vector2 NDCToWorld(const Vector2 &point);
        Vector2 NDCToScreen(const Vector2 &point);
        Vector2 ScreenToNDC(const Vector2 &point);
        Vector2 WorldToScreen(const Vector2 &point);
        Vector2 ScreenToWorld(const Vector2 &point);
    };
}