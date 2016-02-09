#pragma once

namespace ursine
{
    namespace ecs
    {
        enum RenderMask
        {
            Any = 0,
            M0 = 1 << 0,
            M1 = 1 << 1,
            M2 = 1 << 2,
            M3 = 1 << 3,
            M4 = 1 << 4,
            M5 = 1 << 5,
            M6 = 1 << 6,
            M7 = 1 << 7,
            M8 = 1 << 8,
            M9 = 1 << 9,
			Meta(Disable)
            MEditorTool = 1 << 10
        } Meta(Enable);
    }
}