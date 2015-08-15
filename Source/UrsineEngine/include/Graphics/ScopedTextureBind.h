#pragma once

#include "GraphicsConfig.h"

namespace Ursine
{
    class ScopedTextureBind
    {
        GLint _bind;

    public:
        ScopedTextureBind(void);
        ~ScopedTextureBind(void);
    };
}