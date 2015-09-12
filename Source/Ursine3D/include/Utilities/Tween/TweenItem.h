/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionItem.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "DeltaTime.h"

namespace ursine
{
    class TweenItem
    {
    public:
        virtual ~TweenItem(void) {}

        virtual bool Update(DeltaTime dt) = 0;
    };
}