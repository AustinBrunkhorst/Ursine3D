#pragma once

#include "VertexArrayType.h"
#include "Vertex.h"

#include "Drawable.h"

namespace ursine
{
    class VertexArray : public Drawable
    {
        VertexArrayType m_type;

        std::vector<Vertex> m_vertices;

        void Draw(RenderTarget &target, const RenderState &state) const override;

    public:
        VertexArray(void);
        explicit VertexArray(VertexArrayType type, size_t count = 0);

        ~VertexArray(void);

        VertexArrayType GetType(void) const;
        void SetType(VertexArrayType type);

        size_t GetVertexCount(void) const;

        Vertex &operator[](size_t index);
        const Vertex &operator[](size_t index) const;

        void Clear(void);

        void Resize(size_t count);

        void Append(const Vertex &vertex);
    };
}
