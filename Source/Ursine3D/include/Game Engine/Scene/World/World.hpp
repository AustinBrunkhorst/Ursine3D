/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** World.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    namespace ecs
    {
        template<typename SystemType>
        SystemType *World::GetEntitySystem(void)
        {
            if (m_systemManager)
                return m_systemManager->GetSystem<SystemType>( );

            return nullptr;
        }
    }
}
