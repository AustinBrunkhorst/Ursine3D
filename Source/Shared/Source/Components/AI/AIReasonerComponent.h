/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AIReasonerComponent.h
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
        class AIReasoner : public Component
        {
            NATIVE_COMPONENT;

        public:
            AIReasoner(void);

            void OnInitialize(void) override;

            virtual void SensesUpdate(void) {};

            virtual void LogicUpdate(void) {};

        private:
            Handle<Transform> m_trans;
        };

    }
}


