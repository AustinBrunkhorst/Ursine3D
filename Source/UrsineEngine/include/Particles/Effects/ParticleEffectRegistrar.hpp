/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectRegistrar.hpp
** - Register particle effects
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace Ursine
{
    template<class Type>
    void ParticleEffectRegistrar::Register(const char *name)
    {
        static_assert(std::is_base_of<ParticleEffect, Type>::value,
            "The specified type is not a particle effect!");

        ParticleEffectTypeID &id = getNextID();

        // we've reached the max number of bits we can fit in the particle effect type mask.
        // if this becomes a limitation, we can simply change
        // ParticleEffectTypeMask to std::bitset<N> where is N is a larger number
        // of bits than the current typedef. Obviously bitwise operations
        // on integral types are much faster than through the bitset class.

        UAssert(id < kMaxParticleEffectCount, "Maximum number of particle effect types exceeded.");

        // this particle effect type has already been registered
        if (ParticleEffectType<Type>::Registered)
            return;

        ParticleEffectTypeMask mask = 1ull << id;

        ParticleEffectType<Type>::ID = id;
        ParticleEffectType<Type>::Mask = mask;
        ParticleEffectType<Type>::Registered = true;

        auto &t_map = getTypeMap()[id];

        t_map.name = name;
        t_map.mask = mask;

        getOperatorMap()[name] = {
            id,
            {
                static_cast<ParticleEffectOperator::Creator>(ParticleEffectType<Type>::Create),
                static_cast<ParticleEffectOperator::Copier>(ParticleEffectType<Type>::Copy)
            }
        };

        ++id;
    }

    template<class Type>
    void ParticleEffectRegistrar::RegisterMember(const char *member_name,
        ParticleEffectDataAccessor accessor,
        JsonSerializer::Serializer serializer,
        JsonSerializer::Deserializer deserializer)
    {
        static_assert(std::is_base_of<ParticleEffect, Type>::value,
            "The specified type is not a particle effect!");

        UAssert(ParticleEffectType<Type>::Registered,
            "Particle effect type not registered.");

        ParticleEffectTypeID id = ParticleEffectType<Type>::ID;

        auto &reflection = getTypeMap()[id];

        UAssert(reflection.members.find(member_name) == reflection.members.end(),
            "Member already registered.");

        reflection.members[member_name] = std::make_pair(accessor,
            JsonSerializer(serializer, deserializer));
    }

    template<class Type>
    inline ParticleEffectTypeID ParticleEffectRegistrar::GetID(void)
    {
        static_assert(std::is_base_of<ParticleEffect, Type>::value,
            "The specified type is not a particle effect!");

#ifdef CONFIG_DEBUG
        UAssert(ParticleEffectType<Type>::Registered,
            "Particle effect type not registered.");
#endif

        return ParticleEffectType<Type>::ID;
    }

    inline bool ParticleEffectRegistrar::GetMask(const std::string &name, ParticleEffectTypeMask &mask_out)
    {
        auto map = getOperatorMap();
        auto data = map.find(name);

        if (data == map.end())
            return false;

        mask_out = getTypeMap()[data->second.first].mask;

        return true;
    }

    template<class Type>
    inline ParticleEffectTypeMask ParticleEffectRegistrar::GetMask(void)
    {
        static_assert(std::is_base_of<ParticleEffect, Type>::value,
            "The specified type is not a particle effect!");

#ifdef CONFIG_DEBUG
        UAssert(ParticleEffectType<Type>::Registered,
            "Particle effect type not registered.");
#endif

        return ParticleEffectType<Type>::Mask;
    }

    template<class Type>
    inline bool ParticleEffectRegistrar::IsRegistered(void)
    {
        static_assert(std::is_base_of<ParticleEffect, Type>::value,
            "The specified type is not a particle effect!");

        return ParticleEffectType<Type>::Registered;
    }

    inline const ParticleEffectReflection &ParticleEffectRegistrar::Reflect(ParticleEffectTypeID id)
    {
        auto map = getTypeMap();

        UAssert(map.find(id) != map.end(),
            "Type doesn't exist or isn't registered.");

        return getTypeMap()[id];
    }

    inline ParticleEffectRegistrar::TypeMap &ParticleEffectRegistrar::getTypeMap(void)
    {
        static TypeMap map;

        return map;
    }

    inline ParticleEffectRegistrar::OperatorMap &ParticleEffectRegistrar::getOperatorMap(void)
    {
        static OperatorMap map;

        return map;
    }

    inline ParticleEffectTypeID &ParticleEffectRegistrar::getNextID(void)
    {
        static ParticleEffectTypeID id = 0;

        return id;
    }
}