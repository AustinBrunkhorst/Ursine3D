/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EventConfig.h
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
    // Static function delegate
    template<typename Args>
    using StaticDelegate = void(*)(void *, const Args *);

    // Class member function delegate
    template<typename Class, typename Args>
    using ClassDelegate = void(Class::*)(void *, const Args *);
}
