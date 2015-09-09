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

#include "DLLdecl.h"

#include "GFXAPIDefines.h"
#include <DirectXMath.h>
#include <string>

namespace rey_oso
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
    const GFX_API DirectX::XMMATRIX &GetWorldMatrix( void );
    GFX_API void SetWorldMatrix( DirectX::XMMATRIX &matrix );

    GFX_API Model( void );

  private:
    DirectX::XMMATRIX Transform_;
  };

  //3d model class
  class Model3D : public Model
  {
    friend class RenderableManager;
  public:
    const GFX_API char *GetModelName( void );
    GFX_API void SetModel( std::string modelName );

    const GFX_API char *GetMaterialslName( void );
    GFX_API void SetMaterial( std::string materialName );

    GFX_API Model3D( void );

  private:
    std::string ModelName_;
    std::string MaterialName_;
  };

  //2d billboard class
  class Billboard2D : public Model
  {
    friend class RenderableManager;
  public:
    const GFX_API char *GetTextureName( void );
    const GFX_API GFXHND &GetTextureID( void );
    GFX_API void SetTexture( std::string texName );

    GFX_API Billboard2D( void );

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
    GFX_API Primitive( void );
    
    //sets the type, as defined in the enum above
    GFX_API void SetType( PRIMITIVE_TYPE type );
    GFX_API PRIMITIVE_TYPE GetType( );

    //these generic functions do not always apply (setting the radius of a cube does nothing)
    //set radius of sphere or capsule
    GFX_API void SetRadius( float r );
    GFX_API float GetRadius( );

    //set width of cube or plane
    GFX_API void SetWidth( float r );
    GFX_API float GetWidth( );

    //set depth of cube
    GFX_API void SetDepth( float r );
    GFX_API float GetDepth( );

    //set height of cube, plane, or capsule
    GFX_API void SetHeight( float r );
    GFX_API float GetHeight( );

    //renders in wireframe when true, renders flat poly when false
    GFX_API void SetWireFrameMode( bool useWireframe );
    GFX_API bool GetWireFrameMode( );

    //setting the color
    GFX_API void SetColor( DirectX::XMFLOAT4 &color );
    GFX_API void SetColor( float r, float g, float b, float a = 1.0f );
    GFX_API DirectX::XMFLOAT4 &GetColor( );

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
    GFX_API DirectX::XMFLOAT3 &GetDirection( );
    GFX_API void SetDirection( DirectX::XMFLOAT3 &dir );
    GFX_API void SetDirection( float x, float y, float z );

    GFX_API DirectX::XMFLOAT3 &GetColor( );
    GFX_API void SetColor( DirectX::XMFLOAT3 &color );
    GFX_API void SetColor( float r, float g, float b );

    GFX_API DirectionalLight( );

  private:
    DirectX::XMFLOAT3 Direction_;
    DirectX::XMFLOAT3 Color_;
  };

  ///////////////////////////////////////////////////////////////////
  // point light
  class PointLight : public Renderable
  {
  public:
    GFX_API DirectX::XMFLOAT3 &GetPosition( );
    GFX_API void SetPosition( DirectX::XMFLOAT3 &position );
    GFX_API void SetPosition( float x, float y, float z );

    GFX_API DirectX::XMFLOAT3 &GetColor( );
    GFX_API void SetColor( DirectX::XMFLOAT3 &color );
    GFX_API void SetColor( float r, float g, float b );

    GFX_API float &GetRadius( );
    GFX_API void SetRadius( float radius );

    GFX_API PointLight( );

  private:
    DirectX::XMFLOAT3 m_position;
    DirectX::XMFLOAT3 Color_;
    float Radius_;
  };
}