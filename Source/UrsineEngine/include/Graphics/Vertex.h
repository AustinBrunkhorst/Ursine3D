#pragma once

#include "Vector2.h"
#include "Color.h"

namespace Ursine
{
    struct Vertex
    {
        Vector2 position;
        Color color;
        Vector2 uv;

        Vertex(void);
        Vertex(const Vector2 &position);
        Vertex(const Vector2 &position, const Color &color);
        Vertex(const Vector2 &position, const Vector2 &uv);
        Vertex(const Vector2 &position, const Color &color, const Vector2 &uv);
    };
}