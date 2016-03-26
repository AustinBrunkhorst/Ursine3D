/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RasterStateList.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace graphics
    {
        enum RASTER_STATES : unsigned
        {
            RASTER_STATE_SOLID_BACKCULL = 0,
            RASTER_STATE_SOLID_FRONTCULL,
            RASTER_STATE_SOLID_NOCULL,
            RASTER_STATE_WIREFRAME_BACKCULL,
            RASTER_STATE_WIREFRAME_FRONTCULL,
            RASTER_STATE_WIREFRAME_NOCULL,
            RASTER_STATE_LINE_RENDERING,
            RASTER_STATE_SHADOW_RENDER,
            RASTER_STATE_UI,
            RASTER_STATE_COUNT
        };
    }
}
