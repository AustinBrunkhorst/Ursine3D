#include "UrsinePrecompiled.h"

#include "Vertex.h"

namespace Ursine
{
    Vertex::Vertex(void)
        : position(Vec2::Zero())
        , color(Color::White)
        , uv(Vec2::Zero()) { }

    Vertex::Vertex(const Vec2 &position)
        : position(position)
        , color(Color::White)
        , uv(Vec2::Zero()) { }

    Vertex::Vertex(const Vec2 &position, const Color &color)
        : position(position)
        , color(color)
        , uv(Vec2::Zero()) { }

    Vertex::Vertex(const Vec2 &position, const Vec2 &uv)
        : position(position)
        , color(Color::White)
        , uv(uv) { }

    Vertex::Vertex(const Vec2 &position, const Color &color, const Vec2 &uv)
        : position(position)
        , color(color)
        , uv(uv) { }
}
