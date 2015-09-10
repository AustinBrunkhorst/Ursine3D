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

#include "Vec2.h"

#include "AABB.h"

namespace ursine
{
    class Camera2D
    {
        // arbitrary value that's applied to vertical shake, otherwise
        // it looks really odd
        const float VERTICAL_SCALAR = 0.7f;

        // position with applied shake
        Vec2 m_position = Vec2::Zero();

        // position without applied shake
        Vec2 m_positionRaw = Vec2::Zero();

        // target position that we're moving to
        Vec2 m_positionTarget = Vec2::Zero();

        // window size of the application
        const Vec2 &m_viewportSize = gWindowManager->GetSize();

        // current zoom level (relative to the size)
        float m_zoom = 1.0f;

        // target zoom level
        float m_zoomTarget = 1.0f;

        // rotation of the camera
        float m_rotation = 0.0f;

        // speed in which the camera moves to the target
        Vec2 m_speedMove = Vec2::Zero();

        // speed in which the camera shakes
        Vec2 m_speedShake = Vec2::Zero();

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
        Vec2 m_constraintsZoom = Vec2::UnitY();

        // World to camera matrix
        SMat3 m_worldToCamera;

        // Camera to NDC matrix
        SMat3 m_cameraToNDC;

        // World to NDC matrix
        SMat3 m_worldToNDC;

        // NDC to screen matrix
        SMat3 m_ndcToScreen;

        void updatePosition(void);

        void applyBoundConstraints(void);
        void applyZoomConstraints(void);

        void updateMatrices(void);

    public:
        Camera2D(void) = default;

        void Update(void);

        const Vec2 &GetPosition(void);

        void SetTargetPosition(const Vec2 &position);

        const Vec2 &GetRawPosition(void);
        void SetRawPosition(const Vec2 &position);

        float GetZoom(void);
        void SetZoom(float zoom);
        void SetTargetZoom(float zoom);

        const Vec2 &GetMoveSpeed(void);
        void SetMoveSpeed(const Vec2 &speed);

        const Vec2 &GetShakeSpeed(void);
        void SetShakeSpeed(const Vec2 &speed);

        float GetZoomSpeed(void);
        void SetZoomSpeed(float zoom);

        void AddShake(float amount);
        void ClearShake(void);

        const AABB &GetPositionConstraints(void);
        void SetPositionConstraints(const AABB &bounds);
        void SetPositionConstraints(bool constrained);

        const Vec2 &GetZoomConstraints(void);
        void SetZoomConstraints(const Vec2 &constraints);
        void SetZoomConstraints(bool constrained);

        const SMat3 &GetWorldToCamera(void);
        const SMat3 &GetCameraToNDC(void);
        const SMat3 &GetWorldToNDC(void);
        const SMat3 &GetNDCToScreen(void);

        Vec2 WorldToCamera(const Vec2 &point);
        Vec2 CameraToWorld(const Vec2 &point);
        Vec2 WorldToNDC(const Vec2 &point);
        Vec2 NDCToWorld(const Vec2 &point);
        Vec2 NDCToScreen(const Vec2 &point);
        Vec2 ScreenToNDC(const Vec2 &point);
        Vec2 WorldToScreen(const Vec2 &point);
        Vec2 ScreenToWorld(const Vec2 &point);
    };
}