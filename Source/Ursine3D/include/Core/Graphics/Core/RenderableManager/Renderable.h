/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      Renderable.h
Module:         Graphics
Purpose:        All of the possible renderables in graphics
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once



#include "GFXAPIDefines.h"
#include <DirectXMath.h>
#include <string>

namespace ursine
{
  //main renderable class
  class Renderable
  {
    friend class GraphicsCore;
    friend class RenderableManager;
  public:
    Renderable( void );

  private:
    bool Active_;
  };

  //main model class
  class Model : public Renderable
  {
    friend class RenderableManager;
  public:
    const DirectX::XMMATRIX &GetWorldMatrix( void );
    void SetWorldMatrix( DirectX::XMMATRIX &matrix );

    Model( void );

  private:
    DirectX::XMMATRIX Transform_;
  };

  //3d model class
  class Model3D : public Model
  {
    friend class RenderableManager;
  public:
    const char *GetModelName( void );
    void SetModel( std::string modelName );

    const char *GetMaterialslName( void );
    void SetMaterial( std::string materialName );

    Model3D( void );

  private:
    std::string ModelName_;
    std::string MaterialName_;
  };

  //2d billboard class
  class Billboard2D : public Model
  {
    friend class RenderableManager;
  public:
    const char *GetTextureName( void );
    const GFXHND &GetTextureID( void );
    void SetTexture( std::string texName );

    Billboard2D( void );

  private:
    std::string TextureName_;
  };

  //3d primitve class
  class Primitive : public Model
  {
  public:
    enum PRIMITIVE_TYPE
    {
      PRIM_SPHERE = 0,
      PRIM_CUBE,      //done
      PRIM_PLANE,
      PRIM_CAPSULE,   //done
      PRIM_COUNT
    };
  public:
    Primitive( void );

    void SetType( PRIMITIVE_TYPE type );
    PRIMITIVE_TYPE GetType( );

    void SetRadius( float r );
    float GetRadius( );

    void SetWidth( float r );
    float GetWidth( );

    void SetDepth( float r );
    float GetDepth( );

    void SetHeight( float r );
    float GetHeight( );

    void SetWireFrameMode( bool useWireframe );
    bool GetWireFrameMode( );

    DirectX::XMFLOAT4 &GetColor( );
    void SetColor( DirectX::XMFLOAT4 &color );
    void SetColor( float r, float g, float b, float a = 1.0f );
  private:
    PRIMITIVE_TYPE Type_;
    float Radius_;
    float Height_;
    float Width_;
    float Depth_;
    bool UseWireFrame_;
    DirectX::XMFLOAT4 Color_;
  };

  ///////////////////////////////////////////////////////////////////
  // LIGHTS /////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  // directional light
  class DirectionalLight : public Renderable
  {
    friend class RenderableManager;
  public:
    DirectX::XMFLOAT3 &GetDirection( );
    void SetDirection( DirectX::XMFLOAT3 &dir );
    void SetDirection( float x, float y, float z );

    DirectX::XMFLOAT3 &GetColor( );
    void SetColor( DirectX::XMFLOAT3 &color );
    void SetColor( float r, float g, float b );

    DirectionalLight( );

  private:
    DirectX::XMFLOAT3 Direction_;
    DirectX::XMFLOAT3 Color_;
  };

  ///////////////////////////////////////////////////////////////////
  // point light
  class PointLight : public Renderable
  {
  public:
    DirectX::XMFLOAT3 &GetPosition( );
    void SetPosition( DirectX::XMFLOAT3 &position );
    void SetPosition( float x, float y, float z );

    DirectX::XMFLOAT3 &GetColor( );
    void SetColor( DirectX::XMFLOAT3 &color );
    void SetColor( float r, float g, float b );

    float &GetRadius( );
    void SetRadius( float radius );

    PointLight( );

  private:
    DirectX::XMFLOAT3 m_position;
    DirectX::XMFLOAT3 Color_;
    float Radius_;
  };
}