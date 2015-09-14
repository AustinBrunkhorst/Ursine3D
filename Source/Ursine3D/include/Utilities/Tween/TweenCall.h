/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    class TweenCall : public TweenItem
    {
    public:
        TweenCall(TweenCallback callback);

        bool Update(DeltaTime dt) override;

    private:
        TweenCallback m_callback;
    };
}