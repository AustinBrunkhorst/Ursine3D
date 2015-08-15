/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentRegistrar.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ComponentRegistrar.h"
#include "Entity.h"

namespace Ursine
{
    namespace ECS
    {
        Component *ComponentRegistrar::Instantiate(const std::string &name)
        {
            auto map = getOperatorMap();

            // this puppy doesn't exist
            if (map.find(name) == map.end())
                return nullptr;

            return map[name].second.Create();
        }

        Component *ComponentRegistrar::Copy(const Component *instance)
        {
            auto map = getOperatorMap();
            auto &name = instance->GetName();

            // this puppy doesn't exist
            if (map.find(name) == map.end())
                return nullptr;

            return map[name].second.Copy(instance);
        }

        bool ComponentRegistrar::HasDependencies(Entity *entity, std::pair<std::string, std::string> *info)
        {
            auto components = entity->GetComponents();
            auto dependency_map = getDependencyMap();

            for (auto component : components)
            {
                auto &name = component->GetName();

                auto &dependencies = dependency_map[name];

                for (auto dependency : dependencies)
                {
                    auto search = find_if(components.begin(), components.end(), [&dependency](Component *other)
                    {
                        return other->GetName() == dependency;
                    });

                    if (search == components.end())
                    {
                        if (info != nullptr)
                        {
                            (*info).first = name;
                            (*info).second = dependency;
                        }

                        return false;
                    }
                }
            }
        
            return true;
        }
    }
}
