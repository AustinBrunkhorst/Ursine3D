#pragma once

namespace Ursine
{
    enum VertexArrayType
    {
        // List of individual points
        VA_POINTS,
        // List of individual lines
        VA_LINES,
        // List of connected lines, a point uses the previous point to form a line
        VA_LINES_STRIP,
        // List of individual triangles
        VA_TRIANGLES,
        // List of connected triangles, a point uses the two previous points to form a triangle
        VA_TRIANGLES_STRIP,
        // List of connected triangles, a point uses the common center and the previous point to form a triangle
        VA_TRIANGLES_FAN
    };
}