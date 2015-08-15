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

namespace Ursine
{
    class GameplayScreen : public Screen
    {
    protected:
        ECS::World world;

    public:
        GameplayScreen(const std::string &space_name);
        virtual ~GameplayScreen(void) { }

        virtual void OnFocusChanged(bool state);

        virtual void Update(void);
        virtual void Draw(void);
    };
}