/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TriggerCondition.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TriggerCondition.h"

#include "StateMachine.h"

namespace ursine
{
    namespace sm
    {
        TriggerCondition::TriggerCondition(const std::string &variableName)
            : Condition( variableName )
        {
        }

        bool TriggerCondition::GetCondition(StateMachine *stateMachine)
        {
            return stateMachine->GetTrigger( m_variableName );
        }
    }
}
