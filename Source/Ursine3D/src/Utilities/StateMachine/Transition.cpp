/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Transition.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Transition.h"
#include "StateMachine.h"

namespace ursine
{
    namespace sm
    {
        Transition::Transition(const std::string &name)
            : m_name( name )
        {
        }

        bool Transition::CanTransition(StateMachine *stateMachine)
        {
            // return false if any condition is false
            for (auto &condition : m_conditions)
            {
                if (!condition->GetCondition( stateMachine ))
                    return false;
            }

            return true;
        }

        void Transition::RemoveCondition(Condition *condition)
        {
            for (size_t i = 0; i < m_conditions.size( ); ++i)
            {
                if (condition == m_conditions[ i ].get( ))
                {
                    m_conditions.erase( m_conditions.begin( ) + i );
                    return;
                }
            }
        }

        State *Transition::GetDestination(void)
        {
            return m_destination;
        }
    }
}
