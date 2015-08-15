/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScreenFactory.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


namespace Ursine
{
    template<typename ScreenType>
    Screen *ScreenFactory(void)
    {
        static_assert(std::is_base_of<Screen, ScreenType>::value, 
            "type is not a screen!");

        return new ScreenType();
    }
}