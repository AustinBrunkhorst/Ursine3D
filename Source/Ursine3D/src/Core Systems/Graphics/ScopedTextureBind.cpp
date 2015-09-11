#include "UrsinePrecompiled.h"

#include "ScopedTextureBind.h"

namespace ursine
{
    ScopedTextureBind::ScopedTextureBind(void)
    {
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &_bind);
    }

    ScopedTextureBind::~ScopedTextureBind(void)
    {
        glBindTexture(GL_TEXTURE_2D, _bind);
    }
}