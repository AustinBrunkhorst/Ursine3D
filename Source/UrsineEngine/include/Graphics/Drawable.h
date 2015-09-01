#pragma once

#include "RenderState.h"

namespace ursine
{
    class RenderTarget;

    class Drawable
    {
    public:
        virtual ~Drawable(void) { }

        virtual void Draw(RenderTarget &target, const RenderState &states) const = 0;
    };
}