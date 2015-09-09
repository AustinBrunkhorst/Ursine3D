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
#include "DLLdecl.h"
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
  GFX_API void Initialize( void );
  GFX_API void Uninitialize( );

  //getting matrices
  GFX_API DirectX::XMMATRIX GetViewMatrix( );
  GFX_API DirectX::XMMATRIX GetProjMatrix( );

  //setting position
  GFX_API DirectX::XMFLOAT4 GetPosition( );
  GFX_API void SetPosition( const DirectX::XMFLOAT4 &pos );

  //get look direction
  GFX_API DirectX::XMFLOAT4 GetLook( );
  GFX_API void SetLook( const DirectX::XMFLOAT4 &dir );

  //getting right and up
  GFX_API DirectX::XMFLOAT4 GetRight( );
  GFX_API DirectX::XMFLOAT4 GetUp( );

  //field of view
  GFX_API float GetFOV( );
  GFX_API void SetFOV( float fov );

  //projection mode
  GFX_API ProjectionMode GetProjMode( );
  GFX_API void SetProjMode( ProjectionMode mode );

  GFX_API void LookAtPoint( const DirectX::XMFLOAT4 &point );

private:
  void CalculateVectors( DirectX::XMFLOAT4 &up );

  float m_fov;

  ProjectionMode m_projMode;

  DirectX::XMVECTOR m_position;
  DirectX::XMVECTOR m_right, m_up, m_look;
  DirectX::XMMATRIX m_view;
};