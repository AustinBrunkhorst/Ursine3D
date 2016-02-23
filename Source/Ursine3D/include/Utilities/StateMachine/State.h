/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** State.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Transition.h"

#include <string>

namespace ursine
{
    namespace sm
    {
        class State
        {
            friend class StateMachine;

        public:
            typedef std::shared_ptr<State> Handle;

            State(const std::string &name);
            virtual ~State(void) { }

            virtual void OnEnter(StateMachine *machine) = 0;
            virtual void OnUpdate(StateMachine *machine) = 0;
            virtual void OnExit(StateMachine *machine) = 0;

            // This predicate determins if the state can exit this frame
            virtual bool CanExit(void) { return true; }

            // Add transition
            template<typename TransitionType, class... Args>
            TransitionType *AddTransition(State *to, const std::string &transitionName, Args&&... args);

            // Remove transition
            void RemoveTransition(Transition *transition);

        protected:
            // The name of this state
            std::string m_name;

            // The transitions this state has
            std::vector<Transition::Handle> m_transitions;

        };
    }
}

#include "State.hpp"
