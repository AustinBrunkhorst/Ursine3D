/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FloatCondition.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "FloatCondition.h"

#include "StateMachine.h"

namespace ursine
{
    namespace sm
    {
        FloatCondition::FloatCondition(const std::string &variableName, Comparison comparison, float value, float epsilon)
            : Condition( variableName )
            , m_comparison( comparison )
            , m_value( value )
            , m_epsilon( epsilon )
        {
        }

        bool FloatCondition::GetCondition(StateMachine *stateMachine)
        {
            float variable = stateMachine->GetFloat( m_variableName );

            switch (m_comparison)
            {
            case Comparison::LessThan:
                return variable < m_value;
            case Comparison::GreaterThan:
                return variable > m_value;
            case Comparison::Equal:
                return math::IsEqual( variable, m_value, m_epsilon );
            case Comparison::NotEqual:
                return !math::IsEqual( variable, m_value, m_epsilon );
            }

            return false;
        }
    }
}
