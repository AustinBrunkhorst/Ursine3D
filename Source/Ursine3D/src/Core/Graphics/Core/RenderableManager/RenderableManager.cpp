#include "UrsinePrecompiled.h"
#include "RenderableManager.h"

namespace ursine
{
  void RenderableManager::Initialize( )
  {
    m_renderableModel3D.resize( MAX_RENDERABLES );
    m_renderableDirectionalLight.resize( MAX_RENDERABLES );
    m_renderablePointLight.resize( MAX_RENDERABLES );
    m_renderablePrimitives.resize( MAX_RENDERABLES );

    m_handleList.resize( RENDERABLE_TYPE_COUNT );

    for (unsigned x = 0; x < RENDERABLE_TYPE_COUNT; ++x)
    {
      m_handleList[ x ] = new std::list<unsigned>( );
      for (int y = 0; y < MAX_RENDERABLES; ++y)
        m_handleList[ x ]->push_back( y );
    }
  }
  void RenderableManager::Uninitialize( )
  {
    for (unsigned x = 0; x < RENDERABLE_TYPE_COUNT; ++x)
    {
      delete m_handleList[ x ];
    }
  }

  GFXHND RenderableManager::AddRenderable( const RENDERABLE_TYPE type )
  {
    GFXHND data;
    _RENDERABLEHND *newRender = reinterpret_cast<_RENDERABLEHND*>(&data);
    newRender->Index_ = m_handleList[ type ]->front( );
    m_handleList[ type ]->pop_front( );

    switch (type)
    {
    case RENDERABLE_MODEL3D:
      m_renderableModel3D[ newRender->Index_ ].Active_ = true;
      break;
    case RENDERABLE_DIRECTION_LIGHT:
      m_renderableDirectionalLight[ newRender->Index_ ].Active_ = true;
      break;
    case RENDERABLE_POINT_LIGHT:
      m_renderablePointLight[ newRender->Index_ ].Active_ = true;
      break;
    case RENDERABLE_PRIMITIVE:
      m_renderablePrimitives[ newRender->Index_ ].Active_ = true;
      m_renderablePrimitives[ newRender->Index_ ].SetWireFrameMode( true );
      break;
    }

    newRender->Type_ = type;
    newRender->ID_ = ID_RENDERABLE;

    return data;
  }

  void RenderableManager::DestroyRenderable( GFXHND &handle )
  {
    const _RENDERABLEHND *rend = reinterpret_cast<const _RENDERABLEHND*>(&handle);

    UAssert( rend->Index_ != ID_RENDERABLE, "attempted to free a non-valid renderable handle" );
    //UAssert(Renderables_[rend->Type_]->at(rend->Index_).Active_ == true, "Attempted to free an already free handle!");

    m_handleList[ rend->Type_ ]->push_front( rend->Index_ );

    switch (rend->Type_)
    {
    case RENDERABLE_MODEL3D:
      m_renderableModel3D[ rend->Index_ ].Active_ = false;
      break;
    case RENDERABLE_DIRECTION_LIGHT:
      m_renderableDirectionalLight[ rend->Index_ ].Active_ = false;
      break;
    case RENDERABLE_POINT_LIGHT:
      m_renderablePointLight[ rend->Index_ ].Active_ = false;
      break;
    case RENDERABLE_PRIMITIVE:
      m_renderablePrimitives[ rend->Index_ ].Active_ = false;
      break;
    default:
      break;
    }

    handle = 0;
  }
}