/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Filter.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "Filter.h"
#include "Entity.h"

namespace Ursine
{
    namespace ECS
    {
        Filter::Filter(void) 
            : _mask_one(0)
            , _mask_exclude(0)
            , _mask_contains(0) {}

        bool Filter::Matches(const Entity *entity) const
        {
            if (!entity)
                return false;

            return ((_mask_one      & entity->_type_mask) != 0              || _mask_one == 0) &&
                   ((_mask_contains & entity->_type_mask) == _mask_contains || _mask_contains == 0) &&
                   ((_mask_exclude  & entity->_type_mask) == 0              || _mask_exclude == 0);
        }
    }
}