#pragma once

#include "RenderState.h"

namespace Ursine
{
    class RenderTarget;

    class Drawable
    {
    public:
        virtual ~Drawable(void) { }

        virtual void Draw(RenderTarget &target, RenderState states) const = 0;
    };
}