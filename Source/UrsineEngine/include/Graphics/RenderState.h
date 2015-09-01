#pragma once

#include "BlendMode.h"
#include "Texture2D.h"
#include "Shader.h"

namespace ursine
{
    class RenderState
    {
    public:
        RenderState(void);
        RenderState(const BlendMode &blendMode);
        RenderState(const SMat3 &transform);
        RenderState(const Texture2D *texture);
        RenderState(const Shader *shader);
        RenderState(const BlendMode &blendMode, 
            const SMat3 &transform, 
            const Texture2D *texture, 
            const Shader *shader);

        // Default render state
        static const RenderState Default;

        BlendMode blendMode;
        SMat3 transform;
        const Texture2D *texture;
        const Shader *shader;
    };
}
