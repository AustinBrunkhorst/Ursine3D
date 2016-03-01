/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** StateMachine.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "State.h"

#include "Variant.h"

#include <LocalTimerManager.h>

#include <string>
#include <unordered_map>

namespace ursine
{
    namespace sm
    {
        class StateMachine
        {
        public:
            StateMachine(void);
            StateMachine(void *userData);
            virtual ~StateMachine(void) { }

            // Update the state machine
            virtual void Update(void);

            virtual void EnableDebugOutput(std::ostream *output);
            virtual void DisableDebugOutput(void);

            // global variable manipulation
            void AddBool(const std::string &boolName, bool initValue);
            bool GetBool(const std::string &boolName);
            void SetBool(const std::string &boolName, bool value);

            void AddTrigger(const std::string &triggerName);
            bool GetTrigger(const std::string &triggerName);
            void SetTrigger(const std::string &triggerName);

            void AddFloat(const std::string &floatName, float floatValue);
            float GetFloat(const std::string &floatName);
            void SetFloat(const std::string &floatName, float value);

            void AddInt(const std::string &intName, int initValue);
            int GetInt(const std::string &intName);
            void SetInt(const std::string &intName, int value);

            // Remove variable
            void RemoveVariable(const std::string &variableName);

            // Get the user data
            void *GetUserData(void);

            // Get the local timer manager
            LocalTimerManager *GetTimers(void);

            // Add state
            template<typename StateType, class... Args>
            StateType *AddState(Args&&... args);

            // Remove state
            void RemoveState(State *state);

            // Starting state;
            void SetInitialState(State *startingState);

        protected:
            // A map of global variables [name, value]
            std::unordered_map<std::string, meta::Variant> m_variables;
            std::vector<std::string> m_triggers;

            std::vector<State::Handle> m_states;

            LocalTimerManager m_timers;

            bool m_startingState;
            State *m_currentState;

            bool m_transitionFound;
            State *m_nextState;

            void *m_userData;

            bool m_enableDebugOutput;
            std::ostream *m_output;

            State *addState(State::Handle state);

            meta::Variant *getVariable(const std::string &name);

            void printDebugMessage(const std::string &message);

        };
    }
}

#include "StateMachine.hpp"
