/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AIHordelingTypeComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/
#pragma once

#include "Component.h"
#include "TransformComponent.h"


namespace ursine
{
    namespace ecs
    {
        class AIHordelingType : public Component
        {
            NATIVE_COMPONENT;

        public:
            enum HordelingTypeID
            {
                FODDER,
                BOMBER,
                NUKER,
                TANK,
                AGILE,
                INVALID_TYPE
            } Meta(Enable);

            AIHordelingType(void);
            AIHordelingType(HordelingTypeID type);

            HordelingTypeID GetHordelingID(void) const;
            void SetHordelingID(HordelingTypeID type);

        private:
            HordelingTypeID m_type;

        } Meta(Enable, DisplayName("AIType"));

    }
}


