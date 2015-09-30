/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GFXCamera.h
Module:         Graphics
Purpose:        GFXCamera class for viewing 3d world
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "SMat4.h"
#include "ViewportRenderModes.h"

class GFXCamera
{
public:
    enum ProjectionMode
    {
        PROJECTION_PERSPECTIVE = 0,
        PROJECTION_ORTHOGRAPHIC,

        PROJECTION_COUNT
    };

public:
    //initializing
    void Initialize(void);
    void Uninitialize();

    //getting matrices
    ursine::SMat4 GetViewMatrix() const;
    ursine::SMat4 GetProjMatrix(const float width, const float height) const;

    //setting position
    ursine::SVec3 GetPosition() const;
    void SetPosition(const ursine::SVec3 &pos);

    //get look direction
    ursine::SVec3 GetLook() const;
    void SetLook(const ursine::SVec3 &dir);

    //getting right and up
    ursine::SVec3 GetRight(void) const;
    ursine::SVec3 GetUp(void) const;

    //near/far planes
    void SetPlanes(const float nearPlane, const float farPlane);
    void GetPlanes(float &nearPlane, float &farPlane) const;

    //field of view
    float GetFOV(void) const;
    void SetFOV(float fov);

    //projection mode
    ProjectionMode GetProjMode(void) const;
    void SetProjMode(const ProjectionMode mode);

    //orthographic size
    void SetSize(const float size);

    //look at point in world space
    void LookAtPoint(const ursine::SVec3 &point);

    //set dimensions w/ respect to main viewport from 0 to 1
    void SetDimensions(const float width, const float height);
    void GetDimensions(float &width, float &height) const;

    //set the position w/ respect to main viewport
    //0, 0 is top left corner
    void SetPosition(const float x, const float y);
    void GetPosition(float &x, float &y) const;

    //set render mode (orthographics VS perspective)
    void SetRenderMode( const ViewportRenderMode renderMode );

    //get the render mode
    ViewportRenderMode GetRenderMode(void) const;
private:
    void CalculateVectors(const ursine::SVec3 &up);

    float m_fov;
    float m_nearPlane;
    float m_farPlane;

    float m_size;

    ProjectionMode m_projMode;

    ursine::SVec3 m_position;
    ursine::SVec3 m_right, m_up, m_look;
    ursine::SMat4 m_view;

    ViewportRenderMode m_renderMode;

    float m_width;
    float m_height;
    float m_xPos;
    float m_yPos;
};
