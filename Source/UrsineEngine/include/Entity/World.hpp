/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** World.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "Application.h"

namespace ursine
{
    namespace ecs
    {
        inline float World::GetDelta(void) const
        {
            return gApplication->GetDelta();
        }

        inline Camera2D &World::GetCamera(void)
        {
            return _camera;
        }

        template<class ManagerType, typename... Args>
        inline ManagerType *World::addManager(Args&&... args)
        {
            static_assert(std::is_base_of<WorldManager, ManagerType>::value,
                "The specified type is not a manager!");

            static const size_t hash_code = typeid(ManagerType).hash_code();

            UAssert(_managers.find(hash_code) == _managers.end(),
                "This manager type already exists.");

            auto *manager = new ManagerType(this, std::forward<Args>(args)...);

            _managers[hash_code] = manager;

            _manager_queue.push_back(manager);

            return manager;
        }

        template<class ManagerType>
        ManagerType *World::Manager(void)
        {
            static_assert(std::is_base_of<WorldManager, ManagerType>::value,
                "The specified type is not a manager!");

            static const size_t hash_code = typeid(ManagerType).hash_code();

            return static_cast<ManagerType*>(_managers[hash_code]);
        }

        template<typename OwnerType>
        OwnerType *World::GetOwner(void)
        {
            return reinterpret_cast<OwnerType*>(_owner);
        }
    }
}