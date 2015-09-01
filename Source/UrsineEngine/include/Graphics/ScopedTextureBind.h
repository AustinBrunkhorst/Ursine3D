#pragma once

#include "GraphicsConfig.h"

namespace ursine
{
    class ScopedTextureBind
    {
        GLint _bind;

    public:
        ScopedTextureBind(void);
        ~ScopedTextureBind(void);
    };
}