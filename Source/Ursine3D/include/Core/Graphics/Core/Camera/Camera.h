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

#include <DirectXMath.h>

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
  DirectX::XMMATRIX GetViewMatrix( );
  DirectX::XMMATRIX GetProjMatrix( float width, float height );

  //setting position
  DirectX::XMFLOAT4 GetPosition( );
  void SetPosition( const DirectX::XMFLOAT4 &pos );

  //get look direction
  DirectX::XMFLOAT4 GetLook( );
  void SetLook( const DirectX::XMFLOAT4 &dir );

  //getting right and up
  DirectX::XMFLOAT4 GetRight( );
  DirectX::XMFLOAT4 GetUp( );

  //field of view
  float GetFOV( );
  void SetFOV( float fov );

  //projection mode
  ProjectionMode GetProjMode( );
  void SetProjMode( ProjectionMode mode );

  void LookAtPoint( const DirectX::XMFLOAT4 &point );

private:
  void CalculateVectors( DirectX::XMFLOAT4 &up );

  float m_fov;

  ProjectionMode m_projMode;

  DirectX::XMVECTOR m_position;
  DirectX::XMVECTOR m_right, m_up, m_look;
  DirectX::XMMATRIX m_view;
};