#include "UrsinePrecompiled.h"

#include "RenderState.h"

namespace Ursine
{
    RenderState::RenderState(void)
        : blendMode( BlendAlpha )
        , texture( nullptr )
        , shader( nullptr )
    {
        
    }

    RenderState::RenderState(const BlendMode &blendMode)
        : blendMode( blendMode )
        , texture( nullptr )
        , shader( nullptr )
    {
        
    }

    RenderState::RenderState(const Mat3 &transform)
        : blendMode( BlendAlpha )
        , transform( transform )
        , texture( nullptr )
        , shader( nullptr )
    {
        
    }

    RenderState::RenderState(const Texture2D *texture)
        : blendMode( BlendAlpha )
        , texture( texture )
        , shader( nullptr )
    {
        
    }

    RenderState::RenderState(const Shader *shader)
        : blendMode( BlendAlpha )
        , texture( nullptr )
        , shader( shader )
    {
        
    }

    RenderState::RenderState(const BlendMode &blendMode, const Mat3 &transform, const Texture2D *texture, const Shader *shader)
        : blendMode( blendMode )
        , transform( transform )
        , texture( texture )
        , shader( shader )
    {
        
    }
}
