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

namespace Ursine
{
    namespace ECS
    {
        template<class Type>
        Type *SystemManager::AddSystem(void)
        {
            static_assert(std::is_base_of<EntitySystem, Type>::value,
                "The specified type is not an entity system!");

            auto &system = _systems[static_cast<uint>(GetEntitySystemID(Type))];

            UAssert(system == nullptr, "System already added to world.");

            system = SystemType<Type>::Factory(_world);

            system->_type_id = GetEntitySystemID(Type);
            system->_type_mask = GetEntitySystemMask(Type);

            system->OnInitialize();

            return static_cast<Type*>(system);
        }

        template<class SystemType>
        SystemType *SystemManager::System(void)
        {
            static_assert(std::is_base_of<EntitySystem, SystemType>::value,
                "The specified type is not an entity system!");

            return static_cast<SystemType*>(_systems[
                static_cast<uint>(GetEntitySystemID(SystemType))]);
        }
    }
}
