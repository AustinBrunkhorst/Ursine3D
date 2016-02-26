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

#include <CoreSystem.h>
#include <TimerManager.h>

#define DEBUG_OUTPUT(str) if (m_enableDebugOutput) printDebugMessage( str );

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
            , m_timers( GetCoreSystem( TimerManager ) )
            , m_enableDebugOutput( false )
            , m_output( nullptr )
        {
        }

        StateMachine::StateMachine(void *userData)
            : StateMachine( )
        {
            m_userData = userData;
        }

        void StateMachine::Update(void)
        {
            UAssert( m_currentState, "Error: This should never happen.  Did you forget to set a start state?" );

            // Update / Start the current state
            if (m_startingState)
            {
                DEBUG_OUTPUT( "[Begin] OnEnter For State \"" + m_currentState->m_name + "\"" );

                m_currentState->OnEnter( this );
                m_startingState = false;

                DEBUG_OUTPUT( "[End] OnEnter For State \"" + m_currentState->m_name + "\"" );

                // Early out, assuring states go through all three states
                return;
            }
            else
            {
                DEBUG_OUTPUT( "[Begin] OnUpdate For State \"" + m_currentState->m_name + "\"" );

                m_currentState->OnUpdate( this );

                DEBUG_OUTPUT( "[End] OnUpdate For State \"" + m_currentState->m_name + "\"" );
            }

            // If we've already found a transition, and am waiting on a state to finish
            if (m_transitionFound)
            {
                DEBUG_OUTPUT( "Transition Found Last Frame" );

                if (m_currentState->CanExit( ))
                {
                    DEBUG_OUTPUT( "[Begin] OnExit For State \"" + m_currentState->m_name + "\"" );

                    m_currentState->OnExit( this );

                    DEBUG_OUTPUT( "[End] OnExit For State \"" + m_currentState->m_name + "\"" );

                    DEBUG_OUTPUT( "[" + m_currentState->m_name + " -> " + m_nextState->m_name + "]" );

                    m_currentState = m_nextState;
                    m_startingState = true;

                    m_transitionFound = false;
                    m_nextState = nullptr;
                }
                else
                    DEBUG_OUTPUT( "State \"" + m_currentState->m_name + "\" Cannot Exit" );
            }
            else
            {
                // Check for valid transition
                auto &transitions = m_currentState->m_transitions;

                for (auto &transition : transitions)
                {
                    if (transition->CanTransition( this ))
                    {
                        DEBUG_OUTPUT( "Transition Found Named \"" + transition->m_name + "\"" );

                        // We've found a valid transition.  Can the state exit?
                        if (!m_currentState->CanExit( ))
                        {
                            DEBUG_OUTPUT( "State \"" + m_currentState->m_name + "\" Cannot Exit" );

                            m_transitionFound = true;
                            m_nextState = transition->GetDestination( );

                            DEBUG_OUTPUT( "Waiting To Transition To \"" + m_nextState->m_name + "\"" );
                        }
                        else
                        {
                            DEBUG_OUTPUT( "[Begin] OnExit For State \"" + m_currentState->m_name + "\"" );

                            // We have a valid transition, end the current and start the new
                            m_currentState->OnExit( this );

                            DEBUG_OUTPUT( "[End] OnExit For State \"" + m_currentState->m_name + "\"" );

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

        void StateMachine::EnableDebugOutput(std::ostream *output)
        {
            m_enableDebugOutput = true;
            m_output = output;
        }

        void StateMachine::DisableDebugOutput(void)
        {
            m_enableDebugOutput = false;
            m_output = nullptr;
        }

        void StateMachine::AddBool(const std::string &boolName, bool initValue)
        {
            m_variables[ boolName ] = initValue;
        }

        bool StateMachine::GetBool(const std::string &boolName)
        {
            auto variant = getVariable( boolName );

            UAssert( variant->GetType( ) == typeof(bool), "Error: The expected type is missmatched." );

            DEBUG_OUTPUT( "Getting Bool \"" + boolName + "\" Of Value: " + ( variant->ToBool( ) ? "True" : "False" ) );

            return variant->ToBool( );
        }

        void StateMachine::SetBool(const std::string &boolName, bool value)
        {
            auto variant = getVariable( boolName );

            UAssert( variant->GetType( ) == typeof(bool), "Error: The expected type is missmatched." );

            DEBUG_OUTPUT( "Setting Bool \"" + boolName + "\" To Value: " + ( value ? "True" : "False" ) );

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

            DEBUG_OUTPUT( "Getting Trigger \"" + triggerName + "\" Of Value: " + ( variant->ToBool( ) ? "True" : "False" ) );

            return variant->ToBool( );
        }

        void StateMachine::SetTrigger(const std::string &triggerName)
        {
            auto variant = getVariable( triggerName );

            UAssert( variant->GetType( ) == typeof(bool), "Error: The expected type is missmatched." );

            DEBUG_OUTPUT( "Setting Trigger \"" + triggerName + "\"" );

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

            DEBUG_OUTPUT( "Getting Float \"" + floatName + "\" Of Value: " + std::to_string( variant->ToFloat( ) ) );

            return variant->ToFloat( );
        }

        void StateMachine::SetFloat(const std::string &floatName, float value)
        {
            auto variant = getVariable( floatName );

            UAssert( variant->GetType( ) == typeof(float), "Error: The expected type is missmatched." );

            DEBUG_OUTPUT( "Setting Float \"" + floatName + "\" To Value: " + std::to_string( value ) );

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

            DEBUG_OUTPUT( "Getting Int \"" + intName + "\" Of Value: " + std::to_string( variant->ToInt( ) ) );

            return variant->ToInt( );
        }

        void StateMachine::SetInt(const std::string &intName, int value)
        {
            auto variant = getVariable( intName );

            UAssert( variant->GetType( ) == typeof(int), "Error: The expected type is missmatched." );

            DEBUG_OUTPUT( "Setting Int \"" + intName + "\" To Value: " + std::to_string( value ) );

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

        LocalTimerManager *StateMachine::GetTimers(void)
        {
            return &m_timers;
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

        void StateMachine::printDebugMessage(const std::string &message)
        {
            *m_output << "State Machine Message: " << message << std::endl;
        }
    }
}
