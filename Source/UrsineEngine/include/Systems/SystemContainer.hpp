/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemContainer.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    template<class SystemType, class... Args>
    SystemType *SystemContainer::AddSystem(Args&&... args)
    {
        static const size_t hash_code = typeid( SystemType ).hash_code( );

        UAssert(m_systems.find( hash_code ) == m_systems.end( ),
            "System already exists.");

        auto *system = new SystemType( std::forward<Args>( args )... );

        system->m_owner = this;

        m_systems[ hash_code ] = system;

        m_systemQueue.push_back( system );

        return system;
    }
}