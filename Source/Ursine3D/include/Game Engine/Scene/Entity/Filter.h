/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Filter.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ComponentConfig.h"

namespace ursine
{
    namespace ecs
    {
        // forward declarations
        class Entity;

        class Filter
        {
        public:
            Filter(void);

            inline ComponentTypeMask OneMask(void) const;
            inline ComponentTypeMask ExcludeMask(void) const;
            inline ComponentTypeMask ContainsMask(void) const;

            bool Matches(const Entity *entity) const;

            template<class... Types>
            Filter &All(void);

            template<class... Types>
            Filter &Exclude(void);

            template<class... Types>
            Filter &One(void);

        private:
            ComponentTypeMask m_maskOne;
            ComponentTypeMask m_maskExclude;
            ComponentTypeMask m_maskContains;
        };
    }
}

#include "Filter.hpp"
