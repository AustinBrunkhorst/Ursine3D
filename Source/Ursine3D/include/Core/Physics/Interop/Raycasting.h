/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Raycasting.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SVec3.h"
#include "EntityConfig.h"

namespace ursine
{
    namespace physics
    {
        enum RaycastType
        {
            RAYCAST_CLOSEST_HIT,
            RAYCAST_ALL_HITS,
            RAYCAST_NUM
        };

        struct RaycastInput
        {
            SVec3 start, end;

            RaycastInput(void)
                : start( SVec3( ) ), end( SVec3( ) )  { }

            RaycastInput(const SVec3 &start, const SVec3 &end)
                : start( start ), end( end ) { }
        };
        
        struct RaycastOutput
        {
            std::vector<SVec3> hit, normal;
            std::vector<ecs::EntityUniqueID> entity;
        };
    }
}