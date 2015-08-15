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

#include "UrsinePrecompiled.h"

#include "TweenCall.h"

namespace Ursine
{
    TweenCall::TweenCall(TweenCallback callback) 
        : _callback(callback) { }

    bool TweenCall::Update(void)
    {
        _callback();

        return true;
    }
}