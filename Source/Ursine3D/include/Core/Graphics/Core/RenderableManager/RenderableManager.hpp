namespace ursine
{
    //template<typename T>
    //T &RenderableManager::GetRenderable( GfxHND handle )
    //{
    //  return T( );
    //}

    //template<>
    //Model3D &RenderableManager::GetRenderable<Model3D>( GfxHND handle )
    //{
    //  _RENDERABLEHND *render = HND_RENDER( handle );

    //  UAssert( render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!" );
    //  UAssert( render->Type_ == RENDERABLE_MODEL3D, "Attempted to use invalid handle to get a model!" );

    //  return m_renderableModel3D[ render->Index_ ];
    //}

    //template<>
    //DirectionalLight &RenderableManager::GetRenderable<DirectionalLight>( GfxHND handle )
    //{
    //  _RENDERABLEHND *render = HND_RENDER( handle );

    //  UAssert( render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!" );
    //  UAssert( render->Type_ == RENDERABLE_DIRECTION_LIGHT, "Attempted to use invalid handle to get a directional light!" );

    //  return m_renderableDirectionalLight[ render->Index_ ];
    //}

    //template<>
    //PointLight &RenderableManager::GetRenderable<PointLight>( GfxHND handle )
    //{
    //  _RENDERABLEHND *render = HND_RENDER( handle );

    //  UAssert( render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!" );
    //  UAssert( render->Type_ == RENDERABLE_POINT_LIGHT, "Attempted to use invalid handle to get a point light!" );

    //  return m_renderablePointLight[ render->Index_ ];
    //}

    //template<>
    //Primitive &RenderableManager::GetRenderable<Primitive>( GfxHND handle )
    //{
    //  _RENDERABLEHND *render = HND_RENDER( handle );

    //  UAssert( render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!" );
    //  UAssert( render->Type_ == RENDERABLE_PRIMITIVE, "Attempted to use invalid handle to get a primitive!" );

    //  return m_renderablePrimitives[ render->Index_ ];
    //}
}
