/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentRegistrar.hpp
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
        template<class Type>
        void ComponentRegistrar::Register(const char *name)
        {
            static_assert(std::is_base_of<Component, Type>::value,
                "The specified type is not a component!");

            ComponentTypeID &id = getNextID();

            // we've reached the max number of bits we can fit in the component type mask.
            // if this becomes a limitation, we can simply change
            // ComponentTypeMask to std::bitset<N> where is N is a larger number
            // of bits than the current typedef. Obviously bitwise operations
            // on integral types are much faster than through the bitset class.
            
            UAssert(id < kMaxComponentCount, "Maximum number of component types exceeded.");

            // this component type has already been registered
            if (ComponentType<Type>::Registered)
                return;

            ComponentTypeMask mask = 1ull << id;

            ComponentType<Type>::ID = id;
            ComponentType<Type>::Mask = mask;
            ComponentType<Type>::Registered = true;

            auto &t_map = getTypeMap()[id];

            t_map.name = name;
            t_map.mask = mask;

            getOperatorMap()[name] = { 
                id, 
                { 
                    static_cast<ComponentOperator::Creator>(ComponentType<Type>::Create), 
                    static_cast<ComponentOperator::Copier>(ComponentType<Type>::Copy)
                } 
            };

            getDependencyMap()[name] = std::vector<std::string>();

            ++id;
        }

        template<class Type>
        void ComponentRegistrar::RegisterMember(const char *member_name,
            ComponentDataAccessor accessor,
            JsonSerializer::Serializer serializer,
            JsonSerializer::Deserializer deserializer)
        {
            static_assert(std::is_base_of<Component, Type>::value,
                "The specified type is not a component!");

            UAssert(ComponentType<Type>::Registered, 
                "Component type not registered.");

            ComponentTypeID id = ComponentType<Type>::ID;

            auto &reflection = getTypeMap()[id];

            UAssert(reflection.members.find(member_name) == reflection.members.end(),
                "Member already registered.");

            reflection.members[member_name] = std::make_pair(accessor,
                JsonSerializer(serializer, deserializer));
        }

        template<class Type>
        void ComponentRegistrar::RegisterDependency(const char *dependency_name)
        {
            static_assert(std::is_base_of<Component, Type>::value,
                "The specified type is not a component!");

            UAssert(ComponentType<Type>::Registered,
                "Component type not registered.");

            ComponentTypeID id = ComponentType<Type>::ID;

            auto &name = getTypeMap()[id].name;

            getDependencyMap()[name].push_back(dependency_name);
        }

        template<class Type>
        inline ComponentTypeID ComponentRegistrar::GetID(void)
        {
            static_assert(std::is_base_of<Component, Type>::value,
                "The specified type is not a component!");

#ifdef CONFIG_DEBUG
            UAssert(ComponentType<Type>::Registered,
                "Component type not registered.");
#endif

            return ComponentType<Type>::ID;
        }

        inline bool ComponentRegistrar::GetMask(const std::string &name, ComponentTypeMask &mask_out)
        {
            auto map = getOperatorMap();
            auto data = map.find(name);

            if (data == map.end())
                return false;

            mask_out = getTypeMap()[data->second.first].mask;

            return true;
        }

        template<class Type>
        inline ComponentTypeMask ComponentRegistrar::GetMask(void)
        {
            static_assert(std::is_base_of<Component, Type>::value,
                "The specified type is not a component!");

#ifdef CONFIG_DEBUG
            UAssert(ComponentType<Type>::Registered,
                "Component type not registered.");
#endif

            return ComponentType<Type>::Mask;
        }

        template<class Type>
        inline bool ComponentRegistrar::IsRegistered(void)
        {
            static_assert(std::is_base_of<Component, Type>::value,
                "The specified type is not a component!");

            return ComponentType<Type>::Registered;
        }

        inline const ComponentReflection &ComponentRegistrar::Reflect(ComponentTypeID id)
        {
            auto map = getTypeMap();

            UAssert(map.find(id) != map.end(),
                "Type doesn't exist or isn't registered.");

            return getTypeMap()[id];
        }

        inline ComponentRegistrar::TypeMap &ComponentRegistrar::getTypeMap(void)
        {
            static TypeMap map;

            return map;
        }

        inline ComponentRegistrar::OperatorMap &ComponentRegistrar::getOperatorMap(void)
        {
            static OperatorMap map;

            return map;
        }

        inline ComponentRegistrar::DependencyMap &ComponentRegistrar::getDependencyMap(void)
        {
            static DependencyMap map;

            return map;
        }

        inline ComponentTypeID &ComponentRegistrar::getNextID(void)
        {
            static ComponentTypeID id = 0;

            return id;
        }
    }
}