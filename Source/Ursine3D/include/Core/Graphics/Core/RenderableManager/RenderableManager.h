/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      RenderableManager.h
Module:         Graphics
Purpose:        Class manager for holding all renderables
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <vector>
#include <list> 

#include "GraphicsDefines.h"
#include "GfxHandle.h"
#include "RenderableTypes.h"
#include "Renderable.h"

namespace ursine
{
  class Model3D;

  class RenderableManager
  {
    friend class GraphicsCore;
  public:
    void Initialize( );
    void Uninitialize( );

    GFXHND AddRenderable( const RENDERABLE_TYPE type );
    void DestroyRenderable( GFXHND &handle );

    template<typename T>
    T &GetRenderable( GFXHND handle )
    {
      return T( );
    }

    template<>
    Model3D &GetRenderable<Model3D>( GFXHND handle )
    {
      _RENDERABLEHND *render = HND_RENDER( handle );

      UAssert( render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!" );
      UAssert( render->Type_ == RENDERABLE_MODEL3D, "Attempted to use invalid handle to get a model!" );

      return m_renderableModel3D[ render->Index_ ];
    }

    template<>
    DirectionalLight &GetRenderable<DirectionalLight>( GFXHND handle )
    {
      _RENDERABLEHND *render = HND_RENDER( handle );

      UAssert( render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!" );
      UAssert( render->Type_ == RENDERABLE_DIRECTION_LIGHT, "Attempted to use invalid handle to get a directional light!" );

      return m_renderableDirectionalLight[ render->Index_ ];
    }

    template<>
    PointLight &GetRenderable<PointLight>( GFXHND handle )
    {
      _RENDERABLEHND *render = HND_RENDER( handle );

      UAssert( render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!" );
      UAssert( render->Type_ == RENDERABLE_POINT_LIGHT, "Attempted to use invalid handle to get a point light!" );

      return m_renderablePointLight[ render->Index_ ];
    }

    template<>
    Primitive &GetRenderable<Primitive>( GFXHND handle )
    {
      _RENDERABLEHND *render = HND_RENDER( handle );

      UAssert( render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!" );
      UAssert( render->Type_ == RENDERABLE_PRIMITIVE, "Attempted to use invalid handle to get a primitive!" );

      return m_renderablePrimitives[ render->Index_ ];
    }

  private:
    //all the renderables
    std::vector<Model3D> m_renderableModel3D;
    std::vector<DirectionalLight> m_renderableDirectionalLight;
    std::vector<PointLight> m_renderablePointLight;
    std::vector<Primitive> m_renderablePrimitives;

    //TEMPORARY all the lights of the current render
    //std::vector<PointLight *> currentPointLights;
    //std::vector<DirectionalLight *> currentDirectionalLights;

    //all the free handles
    std::vector<std::list<unsigned>*> m_handleList;

  };
}

#include "RenderableManager.hpp"