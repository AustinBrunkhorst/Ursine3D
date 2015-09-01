#pragma once

#include "GraphicsConfig.h"

namespace ursine
{
    typedef GLuint TextureID;

    enum TextureAxis
    {
        UV_U,
        UV_V
    };

    enum TextureWrapMode 
    {
        TW_REPEAT,
        TW_MIRRORED_REPEAT,
        TW_CLAMP_TO_EDGE 
    };

    enum TextureFilterMode 
    {
        TF_NEAREST_NEIGHBOR,
        TF_LINEAR
    };
}