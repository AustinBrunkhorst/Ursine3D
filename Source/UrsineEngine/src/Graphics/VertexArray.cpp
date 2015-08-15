#include "UrsinePrecompiled.h"

#include "VertexArray.h"

#include "RenderTarget.h"

namespace Ursine
{
    VertexArray::VertexArray(void)
        : m_type( VA_POINTS ) { }

    VertexArray::VertexArray(VertexArrayType type, size_t count)
        : m_type( type )
        , m_vertices( count ) { }

    VertexArray::~VertexArray(void) { }

    VertexArrayType VertexArray::GetType(void) const
    {
        return m_type;
    }

    void VertexArray::SetType(VertexArrayType type)
    {
        m_type = type;
    }

    size_t VertexArray::GetVertexCount(void) const
    {
        return m_vertices.size( );
    }

    Vertex &VertexArray::operator[](size_t index)
    {
        return m_vertices[ index ];
    }

    const Vertex &VertexArray::operator[](size_t index) const
    {
        return m_vertices[ index ];
    }

    void VertexArray::Clear(void)
    {
        m_vertices.clear( );
    }

    void VertexArray::Resize(size_t count)
    {
        m_vertices.resize( count );
    }

    void VertexArray::Append(const Vertex &vertex)
    {
        m_vertices.emplace_back( vertex );
    }

    AABB VertexArray::GetBounds(void) const
    {
        AABB bounds;

        // empty AABB
        if (m_vertices.empty())
            return bounds;

        bounds.low_bound = bounds.up_bound = m_vertices[0].position;

        for (size_t i = 1; i < m_vertices.size(); ++i)
        {
            auto position = m_vertices[i].position;

            // update left and right
            if (position.x < bounds.low_bound.x)
                bounds.low_bound.x = position.x;
            else if (position.x > bounds.up_bound.x)
                bounds.up_bound.x = position.x;

            // update top and bottom
            if (position.y < bounds.low_bound.y)
                bounds.low_bound.y = position.y;
            else if (position.y > bounds.up_bound.y)
                bounds.up_bound.y = position.y;
        }

        return bounds;
    }

    void VertexArray::Draw(RenderTarget &target, RenderState state) const
    {
        if (!m_vertices.empty())
            target.Draw( m_vertices.data(), m_vertices.size(), m_type, state );
    }
}
