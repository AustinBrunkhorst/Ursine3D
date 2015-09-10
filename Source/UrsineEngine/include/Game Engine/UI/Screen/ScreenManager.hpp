/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScreenManager.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "ScreenFactory.h"

namespace ursine
{
    template<typename ScreenType>
    void ScreenManager::RegisterScreenFactory(const std::string &name)
    {
        _factories[name] = ScreenFactory<ScreenType>;
    }
}