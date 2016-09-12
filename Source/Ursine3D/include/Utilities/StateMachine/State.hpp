/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** State.hpp
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
        template<typename TransitionType, class... Args>
        TransitionType *State::AddTransition(State *to, const std::string &transitionName, Args&&... args)
        {
            auto transition = std::make_shared<TransitionType>( 
                transitionName, std::forward<Args>( args )... 
            );

            transition->m_destination = to;

            m_transitions.push_back( transition );

            return transition.get( );
        }
    }
}
