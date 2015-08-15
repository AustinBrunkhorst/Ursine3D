/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentRegistrar.h
** - Register component
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineLogTools.h"

#include "ComponentConfig.h"
#include "ComponentOperator.h"
#include "ComponentReflection.h"

#include "JsonSerializer.h"

namespace Ursine
{
    namespace ECS
    {
        class Component;
        class Entity;

        class ComponentRegistrar final
        {
            typedef std::unordered_map<ComponentTypeID, ComponentReflection> TypeMap;
            typedef std::unordered_map<std::string, std::pair<ComponentTypeID, ComponentOperator>> OperatorMap;
            typedef std::unordered_map<std::string, std::vector<std::string>> DependencyMap;

            static TypeMap &getTypeMap(void);
            static OperatorMap &getOperatorMap(void);
            static DependencyMap &getDependencyMap(void);

            static ComponentTypeID &getNextID(void);

        public:
            template<class Type>
            static void Register(const char *name);

            template<class Type>
            static void RegisterMember(const char *member_name, 
                ComponentDataAccessor accessor,
                JsonSerializer::Serializer serializer,
                JsonSerializer::Deserializer deserializer);

            template<class Type>
            static void RegisterDependency(const char *dependency_name);

            // Gets the unique ID for this component type
            template<class Type>
            static inline ComponentTypeID GetID(void);

            // Gets the unique type mask for this component type
            template<class Type>
            static inline ComponentTypeMask GetMask(void);

            // Gets the unique component type mask with this name
            // returns false if the component type doesn't exist
            static inline bool GetMask(const std::string &name, ComponentTypeMask &mask_out);

            template<class Type>
            static inline bool IsRegistered(void);

            // Gets component reflection data for this component type
            static inline const ComponentReflection &Reflect(ComponentTypeID id);

            // Instantiates a component based on its string name
            static Component *Instantiate(const std::string &name);

            // Creates a copy of the component instance
            static Component *Copy(const Component *instance);

            // Determines if an entity's components all contain their dependencies
            // If missing dependencies, "info.first" is set to the component name
            // that is missing a dependency and "info.second" is set to the first 
            // dependency missing
            static bool HasDependencies(Entity *entity, std::pair<std::string, std::string> *info);
        };
    }
}

#include "ComponentRegistrar.hpp"