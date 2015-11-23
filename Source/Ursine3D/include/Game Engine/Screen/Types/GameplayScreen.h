/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GameplayScreen.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Screen.h"

namespace ursine
{
    class GameplayScreen : public Screen
    {
    public:
        GameplayScreen(ScreenManager *manager);
        virtual ~GameplayScreen(void) { }

        virtual void OnFocusChanged(bool state);

        virtual void Update(void);
        virtual void Render(void);

    protected:
        ecs::World::Handle world;
    };
}