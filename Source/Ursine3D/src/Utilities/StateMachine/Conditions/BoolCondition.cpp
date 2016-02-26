/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BoolCondition.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "BoolCondition.h"

#include "StateMachine.h"

namespace ursine
{
    namespace sm
    {
        BoolCondition::BoolCondition(const std::string &variableName, bool value)
            : Condition( variableName )
            , m_value( value )
        {
        }

        bool BoolCondition::GetCondition(StateMachine *stateMachine)
        {
            return m_value == stateMachine->GetBool( m_variableName );
        }
    }
}
