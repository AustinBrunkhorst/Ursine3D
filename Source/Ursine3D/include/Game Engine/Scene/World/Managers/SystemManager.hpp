/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemManager.hpp
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
        template<class SystemType>
        bool SystemManager::HasSystem(void)
        {
            static_assert( std::is_base_of<EntitySystem, SystemType>::value,
                "The type is not a system!" );

            return m_systems.size( ) > SystemType::SystemID && 
                   m_systems[ SystemType::SystemID ] != nullptr;
        }

        template<class SystemType>
        inline SystemType *SystemManager::GetSystem(void)
        {
            static_assert( std::is_base_of<EntitySystem, SystemType>::value,
                "The type is not a system!" );

            return static_cast<SystemType *>( m_systems[ SystemType::SystemID ] );
        }
    }
}
