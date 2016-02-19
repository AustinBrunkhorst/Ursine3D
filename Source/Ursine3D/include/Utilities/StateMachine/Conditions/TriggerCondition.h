/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TriggerCondition.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Condition.h"

namespace ursine
{
    namespace sm
    {
        class TriggerCondition : public Condition
        {
        public:
            TriggerCondition(const std::string &variableName);

            bool GetCondition(StateMachine *stateMachine) override;
        };
    }
}
