/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Transition.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Condition.h"

#include "Array.h"

namespace ursine
{
    namespace sm
    {
        class Transition
        {
            friend class State;
            friend class StateMachine;

        public:
            typedef std::shared_ptr<Transition> Handle;

            Transition(const std::string &name);
            virtual ~Transition(void) { }

            // See if this transition is valid
            virtual bool CanTransition(StateMachine *stateMachine);

            // Add condition
            template<typename ConditionType, class... Args>
            ConditionType *AddCondition(Args&&... args);

            // Remove condition
            void RemoveCondition(Condition *condition);

            State *GetDestination(void);

        protected:
            std::string m_name;

            // This transition's destination
            State *m_destination;

            std::vector<Condition::Handle> m_conditions;
        };
    }
}

#include "Transition.hpp"
