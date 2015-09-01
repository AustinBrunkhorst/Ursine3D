#pragma once

#include "Vec2.h"
#include "Color.h"

namespace ursine
{
    struct Vertex
    {
        Vec2 position;
        Color color;
        Vec2 uv;

        Vertex(void);
        Vertex(const Vec2 &position);
        Vertex(const Vec2 &position, const Color &color);
        Vertex(const Vec2 &position, const Vec2 &uv);
        Vertex(const Vec2 &position, const Color &color, const Vec2 &uv);
    };
}