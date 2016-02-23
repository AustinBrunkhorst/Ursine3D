/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** IntCondition.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "IntCondition.h"

#include "StateMachine.h"

namespace ursine
{
    namespace sm
    {
        IntCondition::IntCondition(const std::string &variableName, Comparison comparison, int value)
            : Condition( variableName )
            , m_comparison( comparison )
            , m_value( value )
        {
        }

        bool IntCondition::GetCondition(StateMachine *stateMachine)
        {
            auto variable = stateMachine->GetInt( m_variableName );

            switch (m_comparison)
            {
                case Comparison::Equal:
                    return m_value == variable;
                case Comparison::LessThan:
                    return variable < m_value;
                case Comparison::GreaterThan:
                    return variable > m_value;
                case Comparison::NotEqual:
                    return variable != m_value;
            }

            return false;
        }
    }
}
