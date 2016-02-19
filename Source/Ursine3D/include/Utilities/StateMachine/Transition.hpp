/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Transition.hpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    namespace sm
    {
        template<typename ConditionType, class... Args>
        ConditionType *Transition::AddCondition(const std::string &variableName, Args&&... args)
        {
            auto condition = std::make_shared<ConditionType>(
                variableName, std::forward<Args>( args )...
            );

            m_conditions.push_back( condition );

            return condition.get( );
        }
    }
}
