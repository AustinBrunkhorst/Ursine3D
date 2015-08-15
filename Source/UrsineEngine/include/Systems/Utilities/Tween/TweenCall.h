/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionCall.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "TweenItem.h"
#include "TweenConfig.h"

namespace Ursine
{
    class TweenCall : public TweenItem
    {
        TweenCallback _callback;

    public:
        TweenCall(TweenCallback callback);

        bool Update(void) override;
    };
}