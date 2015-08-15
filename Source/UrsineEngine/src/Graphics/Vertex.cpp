#include "UrsinePrecompiled.h"

#include "Vertex.h"

namespace Ursine
{
    Vertex::Vertex(void)
        : position(Vector2::Zero())
        , color(Color::White)
        , uv(Vector2::Zero()) { }

    Vertex::Vertex(const Vector2 &position)
        : position(position)
        , color(Color::White)
        , uv(Vector2::Zero()) { }

    Vertex::Vertex(const Vector2 &position, const Color &color)
        : position(position)
        , color(color)
        , uv(Vector2::Zero()) { }

    Vertex::Vertex(const Vector2 &position, const Vector2 &uv)
        : position(position)
        , color(Color::White)
        , uv(uv) { }

    Vertex::Vertex(const Vector2 &position, const Color &color, const Vector2 &uv)
        : position(position)
        , color(color)
        , uv(uv) { }
}
