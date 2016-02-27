/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimerCondition.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Condition.h"

#include <Timer.h>

namespace ursine
{
    namespace sm
    {
        class TimerCondition : public Condition
        {
        public:
            TimerCondition(TimeSpan timeSpan);

            bool GetCondition(StateMachine *stateMachine) override;

        private:

            bool m_finished;
            TimerID m_timer;
            TimeSpan m_timeSpan;
        };
    }
}
