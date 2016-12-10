/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ClothNode.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <SVec3.h>

struct ClothNode
{
    ClothNode(void);

    ursine::SVec3 position;
    ursine::SVec3 velocity;
    ursine::SVec3 force;
};
