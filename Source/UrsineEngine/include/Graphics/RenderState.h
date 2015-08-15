#pragma once

#include "BlendMode.h"
#include "Texture2D.h"
#include "Shader.h"

namespace Ursine
{
    class RenderState
    {
    public:
        RenderState(void);
        RenderState(const BlendMode &blendMode);
        RenderState(const Matrix3 &transform);
        RenderState(const Texture2D *texture);
        RenderState(const Shader *shader);
        RenderState(const BlendMode &blendMode, 
            const Matrix3 &transform, 
            const Texture2D *texture, 
            const Shader *shader);

        // Default render state
        static const RenderState Default;

        BlendMode blendMode;
        Matrix3 transform;
        const Texture2D *texture;
        const Shader *shader;
    };
}
