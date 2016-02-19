/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BoolCondition.h
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
        class BoolCondition : public Condition
        {
        public:
            BoolCondition(const std::string &variableName, bool value);

            bool GetCondition(StateMachine *stateMachine) override;

        private:
            bool m_value;
        };
    }
}
