#pragma once

#include "Texture2D.h"

#include "Drawable.h"

#include "BlendMode.h"
#include "RenderState.h"

#include "VertexArrayType.h"
#include "Vertex.h"

namespace Ursine
{
    class RenderTarget
    {
        struct StateCache
        {
            static const int VertexCacheSize = 4;

            // are our internal GL states set yet?
            bool glStatesSet;

            // has the current view changed since last draw?
            bool viewChanged;    

            // cached blending mode
            BlendMode lastBlendMode;  

            // cached texture
            Uint64 lastTextureID;  

            // did we previously use the vertex cache?
            bool useVertexCache; 

            // pre-transformed vertices cache 
            Vertex vertexCache[VertexCacheSize];    
        };

        StateCache m_cache;
        Camera2D m_camera;

        void applyCurrentCamera(void);
        void applyBlendMode(const BlendMode &mode);
        void applyTransform(const Mat3 &transform);
        void applyTexture(const Texture2D *texture);
        void applyShader(const Shader *shader);

    public:
        RenderTarget(void);
        virtual ~RenderTarget(void);

        const Camera2D &GetCamera(void) const;

        void Clear(const Color &color = Color::Black);

        void Draw(const Drawable &drawable, const RenderState &state);
        void Draw(const Vertex *vertices, size_t vertexCount, VertexArrayType type, const RenderState &state);

        void PushGLStates(void);
        void PopGLStates(void);
        void ResetGLStates(void);
    };
}