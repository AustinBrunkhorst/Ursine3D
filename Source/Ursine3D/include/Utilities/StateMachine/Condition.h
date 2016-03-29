/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Condition.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <string>

namespace ursine
{
    namespace sm
    {
        class StateMachine;

        enum class Comparison
        {
            GreaterThan,
            LessThan,
            Equal,
            NotEqual
        };

        class Condition
        {
        public:
            typedef std::shared_ptr<Condition> Handle;

            Condition(const std::string &variableName);
            virtual ~Condition(void) { }

            virtual bool GetCondition(StateMachine *stateMachine) = 0;

            virtual void Reset(StateMachine *stateMachine) { }

        protected:
            std::string m_variableName;

        };
    }
}
