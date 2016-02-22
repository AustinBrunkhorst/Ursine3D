/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** StateMachine.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "StateMachine.h"

#include "Transition.h"

namespace ursine
{
    namespace sm
    {
        StateMachine::StateMachine(void)
            : m_currentState( nullptr )
            , m_startingState( true )
            , m_userData( nullptr )
            , m_transitionFound( false )
            , m_nextState( nullptr )
        {
        }

        StateMachine::StateMachine(void *userData)
            : m_currentState( nullptr )
            , m_startingState( true )
            , m_userData( userData )
            , m_transitionFound( false )
            , m_nextState( nullptr )
        {
        }

        void StateMachine::Update(void)
        {
            UAssert( m_currentState, "Error: This should never happen.  Did you forget to set a start state?" );

            // Update / Start the current state
            if (m_startingState)
            {
                m_currentState->OnEnter( this );
                m_startingState = false;

                // Early out, assuring states go through all three states
                return;
            }
            else
            {
                m_currentState->OnUpdate( this );
            }

            // If we've already found a transition, and am waiting on a state to finish
            if (m_transitionFound)
            {
                if (m_currentState->CanExit( ))
                {
                    m_currentState->OnExit( this );
                    
                    m_currentState = m_nextState;
                    m_startingState = true;

                    m_transitionFound = false;
                    m_nextState = nullptr;
                }
            }
            else
            {
                // Check for valid transition
                auto &transitions = m_currentState->m_transitions;

                for (auto &transition : transitions)
                {
                    if (transition->CanTransition( this ))
                    {
                        // We've found a valid transition.  Can the state exit?
                        if (!m_currentState->CanExit( ))
                        {
                            m_transitionFound = true;
                            m_nextState = transition->GetDestination( );
                        }
                        else
                        {
                            // We have a valid transition, end the current and start the new
                            m_currentState->OnExit( this );

                            m_currentState = transition->GetDestination( );
                            m_startingState = true;
                        }
                    }
                }
            }

            // unset all triggers
            for (auto &triggerName : m_triggers)
            {
                auto *variant = getVariable( triggerName );

                *variant = false;
            }
        }

        void StateMachine::AddBool(const std::string &boolName, bool initValue)
        {
            m_variables[ boolName ] = initValue;
        }

        bool StateMachine::GetBool(const std::string &boolName)
        {
            auto variant = getVariable( boolName );

            UAssert( variant->GetType( ) == typeof(bool), "Error: The expected type is missmatched." );

            return variant->ToBool( );
        }

        void StateMachine::SetBool(const std::string &boolName, bool value)
        {
            auto variant = getVariable( boolName );

            UAssert( variant->GetType( ) == typeof(bool), "Error: The expected type is missmatched." );

            *variant = value;
        }

        void StateMachine::AddTrigger(const std::string &triggerName)
        {
            m_variables[ triggerName ] = false;

            m_triggers.push_back( triggerName );
        }

        bool StateMachine::GetTrigger(const std::string &triggerName)
        {
            auto variant = getVariable( triggerName );

            UAssert( variant->GetType( ) == typeof(bool), "Error: The expected type is missmatched." );

            return variant->ToBool( );
        }

        void StateMachine::SetTrigger(const std::string &triggerName)
        {
            auto variant = getVariable( triggerName );

            UAssert( variant->GetType( ) == typeof(bool), "Error: The expected type is missmatched." );

            *variant = true;
        }

        void StateMachine::AddFloat(const std::string &floatName, float initValue)
        {
            m_variables[ floatName ] = initValue;
        }

        float StateMachine::GetFloat(const std::string &floatName)
        {
            auto variant = getVariable( floatName );

            UAssert( variant->GetType( ) == typeof(float), "Error: The expected type is missmatched." );

            return variant->ToFloat( );
        }

        void StateMachine::SetFloat(const std::string &floatName, float value)
        {
            auto variant = getVariable( floatName );

            UAssert( variant->GetType( ) == typeof(float), "Error: The expected type is missmatched." );

            *variant = value;
        }

        void StateMachine::AddInt(const std::string &intName, int initValue)
        {
            m_variables[ intName ] = initValue;
        }

        int StateMachine::GetInt(const std::string &intName)
        {
            auto variant = getVariable( intName );

            UAssert( variant->GetType( ) == typeof(int), "Error: The expected type is missmatched." );

            return variant->ToInt( );
        }

        void StateMachine::SetInt(const std::string &intName, int value)
        {
            auto variant = getVariable( intName );

            UAssert( variant->GetType( ) == typeof(int), "Error: The expected type is missmatched." );

            *variant = value;
        }

        void StateMachine::RemoveVariable(const std::string &variableName)
        {
            m_variables.erase( m_variables.find( variableName ) );

            for (size_t i = 0; i < m_triggers.size( ); ++i)
            {
                if (m_triggers[ i ] == variableName)
                {
                    m_triggers.erase( m_triggers.begin( ) + i );
                    return;
                }
            }
        }

        void *StateMachine::GetUserData(void)
        {
            return m_userData;
        }

        void StateMachine::RemoveState(State *state)
        {
            for (size_t i = 0; i < m_states.size( ); ++i)
            {
                if (state == m_states[ i ].get( ))
                {
                    m_states.erase( m_states.begin( ) + i );
                    return;
                }
            }
        }

        void StateMachine::SetInitialState(State *initialState)
        {
            m_currentState = initialState;
        }

        State *StateMachine::addState(State::Handle state)
        {
            m_states.push_back( state );

            return state.get( );
        }

        meta::Variant *StateMachine::getVariable(const std::string &name)
        {
            auto itr = m_variables.find( name );

            UAssert( itr != m_variables.end( ), "Error: The variable named %s does not exist.", name.c_str( ) );

            return &m_variables[ name ];
        }
    }
}
