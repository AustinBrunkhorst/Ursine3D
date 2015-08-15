#include "UrsinePrecompiled.h"

#include "ScopedTextureBind.h"

namespace Ursine
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