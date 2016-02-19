/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** State.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "State.h"

namespace ursine
{
    namespace sm
    {
        State::State(const std::string &name)
            : m_name( name )
        {
        }

        void State::RemoveTransition(Transition *transition)
        {
            for (size_t i = 0; i < m_transitions.size( ); ++i)
            {
                if (transition == m_transitions[ i ].get( ))
                {
                    m_transitions.erase( m_transitions.begin( ) + i );
                }
            }
        }
    }
}
