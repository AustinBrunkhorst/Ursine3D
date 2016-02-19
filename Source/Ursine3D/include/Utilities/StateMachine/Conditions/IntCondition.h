/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** IntCondition.h
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
        class IntCondition : public Condition
        {
        public:
            IntCondition(const std::string &variableName, Comparison comparision, int value);

            bool GetCondition(StateMachine *stateMachine) override;

        private:
            Comparison m_comparison;

            int m_value;
        };
    }
}
