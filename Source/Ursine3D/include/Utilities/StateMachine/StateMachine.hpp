/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** StateMachine.hpp
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
        template<typename StateType, class... Args>
        StateType *StateMachine::AddState(Args&&... args)
        {
            return reinterpret_cast<StateType*>( addState( 
                std::make_shared<StateType>( 
                    std::forward<Args>( args )... 
                )
            ) );
        }
    }
}