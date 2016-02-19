/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FloatCondition.h
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
        class FloatCondition : public Condition
        {
        public:
            FloatCondition(const std::string &variableName, Comparison comparison, 
                           float value, float epsilon = 0.0001f);

            bool GetCondition(StateMachine *stateMachine) override;

        private:
            Comparison m_comparison;

            float m_value;

            float m_epsilon;
        };
    }
}
