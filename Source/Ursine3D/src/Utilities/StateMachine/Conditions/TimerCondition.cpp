/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimerCondition.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TimerCondition.h"

#include "StateMachine.h"

namespace ursine
{
    namespace sm
    {
        TimerCondition::TimerCondition(TimeSpan timeSpan)
            : Condition( "NULL" )
            , m_timeSpan( timeSpan )
            , m_finished( false )
        {
        }

        bool TimerCondition::GetCondition(StateMachine *stateMachine)
        {
            if (!m_timer.IsValid( ))
            {
                m_timer = stateMachine->GetTimers( )->Create( m_timeSpan )
                    .Completed( [=] {
                        m_finished = true;
                    } );
            }

            return m_finished;
        }

        void TimerCondition::Reset(StateMachine *machine)
        {
            m_timer.Cancel( );
            m_finished = false;
        }
    }
}
