/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemManagerEvent.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace ecs
    {
        enum SystemManagerEvent
        {
            // The parent world has loaded content (attributes, entities, etc)
            SM_WORLD_LOADED,
            // when the screen that owns this world changes input focus
            SM_SCREEN_FOCUS_CHANGED,
            // systems should be updated only
            SM_UPDATE,
            // systems should be drawn only
            SM_DRAW
        };

        struct ScreenFocusArgs : public EventArgs
        {
            bool focused;

            ScreenFocusArgs(bool focused)
                : focused(focused) { }
        };
    }
}
