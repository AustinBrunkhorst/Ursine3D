/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      Camera.h
Module:         Graphics
Purpose:        Camera class for viewing 3d world
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "SMat4.h"

class Camera
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
  void Initialize( void );
  void Uninitialize( );

  //getting matrices
  ursine::SMat4 GetViewMatrix( );
  ursine::SMat4 GetProjMatrix( float width, float height );

  //setting position
  ursine::SVec3 GetPosition( );
  void SetPosition( const ursine::SVec3 &pos );

  //get look direction
  ursine::SVec3 GetLook( );
  void SetLook( const ursine::SVec3 &dir );

  //getting right and up
  ursine::SVec3 GetRight( );
  ursine::SVec3 GetUp( );

  //near/far planes
  void SetPlanes( float nearPlane, float farPlane );
  void GetPlanes( float &nearPlane, float &farPlane );

  //field of view
  float GetFOV( );
  void SetFOV( float fov );

  //projection mode
  ProjectionMode GetProjMode( );
  void SetProjMode( ProjectionMode mode );

  //orthographic size
  void SetSize( float size );

  void LookAtPoint( const ursine::SVec3 &point );

private:
  void CalculateVectors( const ursine::SVec3 &up );

  float m_fov;
  float m_nearPlane;
  float m_farPlane;

  float m_size;

  ProjectionMode m_projMode;

  ursine::SVec3 m_position;
  ursine::SVec3 m_right, m_up, m_look;
  ursine::SMat4 m_view;
};