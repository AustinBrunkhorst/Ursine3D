#pragma once

namespace ursine
{
    namespace spine
    {
        enum AtlasFormat
        {
            SP_ATLAS_ALPHA,
            SP_ATLAS_INTENSITY,
            SP_ATLAS_LUMINANCE_ALPHA,
            SP_ATLAS_RGB565,
            SP_ATLAS_RGBA4444,
            SP_ATLAS_RGB888,
            SP_ATLAS_RGBA8888
        };

        enum AtlasFilter
        {
            SP_ATLAS_NEAREST,
            SP_ATLAS_LINEAR,
            SP_ATLAS_MIPMAP,
            SP_ATLAS_MIPMAP_NEAREST_NEAREST,
            SP_ATLAS_MIPMAP_LINEAR_NEAREST,
            SP_ATLAS_MIPMAP_NEAREST_LINEAR,
            SP_ATLAS_MIPMAP_LINEAR_LINEAR
        };

        enum AtlasWrap
        {
            SP_ATLAS_MIRROREDREPEAT,
            SP_ATLAS_CLAMPTOEDGE,
            SP_ATLAS_REPEAT
        };
    }
}