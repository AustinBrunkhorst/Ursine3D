#include "UrsinePrecompiled.h"

#include "RenderTarget.h"

namespace
{
    // converts a BlendMode::Factor constant to the corresponding OpenGL constant
    uint32 factorToGlConstant(ursine::BlendMode::Factor blendFactor)
    {
        switch (blendFactor)
        {
        default:
        case ursine::BlendMode::Zero:             
            return GL_ZERO;
        case ursine::BlendMode::One:              
            return GL_ONE;
        case ursine::BlendMode::SrcColor:         
            return GL_SRC_COLOR;
        case ursine::BlendMode::OneMinusSrcColor: 
            return GL_ONE_MINUS_SRC_COLOR;
        case ursine::BlendMode::DstColor:         
            return GL_DST_COLOR;
        case ursine::BlendMode::OneMinusDstColor: 
            return GL_ONE_MINUS_DST_COLOR;
        case ursine::BlendMode::SrcAlpha:         
            return GL_SRC_ALPHA;
        case ursine::BlendMode::OneMinusSrcAlpha: 
            return GL_ONE_MINUS_SRC_ALPHA;
        case ursine::BlendMode::DstAlpha:         
            return GL_DST_ALPHA;
        case ursine::BlendMode::OneMinusDstAlpha: 
            return GL_ONE_MINUS_DST_ALPHA;
        }
    }

    // converts a BlendMode::BlendEquation constant to the corresponding OpenGL constant
    uint32 equationToGlConstant(ursine::BlendMode::Equation blendEquation)
    {
        switch (blendEquation)
        {
        default:
        case ursine::BlendMode::Add:            
            return GL_FUNC_ADD;
        case ursine::BlendMode::Subtract:        
            return GL_FUNC_SUBTRACT;
        }
    }
}

namespace ursine
{
    RenderTarget::RenderTarget(void)
    {
        
    }

    RenderTarget::~RenderTarget(void)
    {
        
    }

    void RenderTarget::Clear(const Color &color)
    {
        applyTexture( nullptr );

        glClearColor( color.r, color.g, color.b, color.a );
        glClear( GL_COLOR_BUFFER_BIT );
    }

    void RenderTarget::Draw(const Drawable &drawable, const RenderState &state)
    {
        drawable.Draw(*this, state);
    }

    void RenderTarget::Draw(const Vertex *vertices, size_t vertexCount, VertexArrayType type, const RenderState &state)
    {
        // nothing to draw
        if (!vertices || vertexCount == 0)
            return;

        // first set the persistent OpenGL states if it's the very first call
        if (!m_cache.glStatesSet)
            ResetGLStates( );

        // check if the vertex count is low enough so that we can pre-transform them
        bool useVertexCache = (vertexCount <= StateCache::VertexCacheSize);

        if (useVertexCache)
        {
            // pre-transform the vertices and store them into the vertex cache
            for (size_t i = 0; i < vertexCount; ++i)
            {
                auto &vertex = m_cache.vertexCache[i];

                vertex.position = state.transform.TransformPoint( vertices[i].position );
                vertex.color = vertices[i].color;
                vertex.uv = vertices[i].uv;
            }

            // since vertices are transformed, 
            // we must use an identity transform to render them
            if (!m_cache.useVertexCache)
                applyTransform( SMat3::Identity( ) );
        }
        else
        {
            applyTransform( state.transform );
        }

        // Apply the view
        if (m_cache.viewChanged)
            applyCurrentCamera( );

        // Apply the blend mode
        if (state.blendMode != m_cache.lastBlendMode)
            applyBlendMode( state.blendMode );

        // Apply the texture
        auto textureId = state.texture ? state.texture->GetUniqueID( ) : 0;

        if (textureId != m_cache.lastTextureID)
            applyTexture( state.texture );

        // apply the shader
        if (state.shader)
            applyShader( state.shader );

        // If we pre-transform the vertices, we must use our internal vertex cache
        if (useVertexCache)
        {
            // ... and if we already used it previously, we don't need to set the pointers again
            if (!m_cache.useVertexCache)
                vertices = m_cache.vertexCache;
            else
                vertices = nullptr;
        }

        // setup the pointers to the vertices' components
        if (vertices)
        {
            auto data = reinterpret_cast<const char*>( vertices );

            glVertexPointer( 2, GL_FLOAT, sizeof(Vertex), data + 0 );
            glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + 8 );
            glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), data + 12 );
        }

        // find the OpenGL primitive type
        static const GLenum modeMap [] = { 
            GL_POINTS, 
            GL_LINES, 
            GL_LINE_STRIP, 
            GL_TRIANGLES,
            GL_TRIANGLE_STRIP, 
            GL_TRIANGLE_FAN, 
            GL_QUADS 
        };

        // draw the primitives
        glDrawArrays( modeMap[type], 0, vertexCount );

        // unbind the shader, if any
        if (state.shader)
            applyShader( nullptr );

        // update the cache
        m_cache.useVertexCache = useVertexCache;
    }

    void RenderTarget::PushGLStates(void)
    {
        glPushClientAttrib( GL_CLIENT_ALL_ATTRIB_BITS );
        glPushAttrib( GL_ALL_ATTRIB_BITS );

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix( );
        glMatrixMode( GL_PROJECTION );
        glPushMatrix( );
        glMatrixMode( GL_TEXTURE );
        glPushMatrix( );
    }

    void RenderTarget::PopGLStates(void)
    {
        glMatrixMode( GL_PROJECTION );
        glPopMatrix( );
        glMatrixMode( GL_MODELVIEW );
        glPopMatrix( );
        glMatrixMode( GL_TEXTURE );
        glPopMatrix( );

        glPopClientAttrib( );
        glPopAttrib( );
    }

    void RenderTarget::ResetGLStates(void)
    {
        // define the default OpenGL states
        glDisable( GL_CULL_FACE );
        glDisable( GL_LIGHTING) ;
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_ALPHA_TEST );
        glEnable( GL_TEXTURE_2D );
        glEnable( GL_BLEND );
        glMatrixMode( GL_MODELVIEW );
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );

        m_cache.glStatesSet = true;

        // apply the default state values
        
        applyBlendMode( BlendAlpha );
        applyTransform( SMat3::Identity() );

        applyTexture( nullptr );
        
        applyShader( nullptr );

        m_cache.useVertexCache = false;

        // set the default view
        //setView(getView());
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Private Methods
    ////////////////////////////////////////////////////////////////////////////////

    void RenderTarget::applyCurrentCamera(void)
    {
        
    }

    void RenderTarget::applyBlendMode(const BlendMode &mode)
    {
        if (GLEW_EXT_blend_func_separate)
        {
            glBlendFuncSeparateEXT(
                factorToGlConstant(mode.colorSrcFactor), 
                factorToGlConstant(mode.colorDstFactor),
                factorToGlConstant(mode.alphaSrcFactor), 
                factorToGlConstant(mode.alphaDstFactor));
        }
        else
        {
            glBlendFunc(
                factorToGlConstant(mode.colorSrcFactor),
                factorToGlConstant(mode.colorDstFactor));
        }

        if (GLEW_EXT_blend_equation_separate)
        {
            glBlendEquationSeparateEXT(
                equationToGlConstant(mode.colorEquation),
                equationToGlConstant(mode.alphaEquation));
        }
        else
        {
            glBlendEquation(
                equationToGlConstant(mode.colorEquation));
        }

        m_cache.lastBlendMode = mode;
    }

    void RenderTarget::applyTransform(const SMat3 &transform)
    {
        glLoadMatrixf( transform.GetFloatPtr( ) );
    }

    void RenderTarget::applyTexture(const Texture2D *texture)
    {
        Texture2D::Bind(texture);

        m_cache.lastTextureID = texture ? texture->GetUniqueID( ) : 0;
    }

    void RenderTarget::applyShader(const Shader *shader)
    {
        Shader::Bind( shader );
    }
}