/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    namespace ecs
    {
        Filter::Filter(void) 
            : m_maskOne( 0 )
            , m_maskExclude( 0 )
            , m_maskContains( 0 ) { }

        bool Filter::Matches(const Entity *entity) const
        {
            if (!entity)
                return false;

            return Matches( entity->m_typeMask );
        }

        bool Filter::Matches(const ComponentTypeMask& typeMask) const
        {
            return ((m_maskOne      & typeMask) != 0              || m_maskOne == 0) &&
                   ((m_maskContains & typeMask) == m_maskContains || m_maskContains == 0) &&
                   ((m_maskExclude  & typeMask) == 0              || m_maskExclude == 0);
        }
    }
}