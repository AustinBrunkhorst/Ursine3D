/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineStateUtils.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "VineAIComponent.h"

class VineStateUtils
{
public:

    // Find the Y position the vine should be at using raycasting
    static float FindYPosition(VineAI *ai, const ursine::SVec3 &aiPosition);

    // Determine if we're at our target
    static bool AtTarget(VineAI *ai, float distance);

    static bool AtHome(VineAI *ai, float distance);

};