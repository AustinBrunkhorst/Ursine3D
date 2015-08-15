/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectConfig.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineTypes.h"
#include "Utils.h"

#include <vector>
#include <unordered_map>

// Registers an effect with the registrar.
// IMPORTANT: this must be done in the effects header file.
#define RegisterParticleEffect(effect_type)                   \
    volatile static Ursine::ParticleEffectType<effect_type>   \
        register_##effect_type(#effect_type);                 \

// Intellisense is a silly guy
#ifdef __INTELLISENSE__
// Registers particle effect data with the registrar.
// IMPORTANT: this must be done in the particle effect's header file.
#define RegisterParticleEffectData(effect_type, member_name, ...)
#else
#define RegisterParticleEffectData(effect_type, member_name, ...)                                  \
    static void *access_##effect_type##member_name(const Ursine::ParticleEffect *instance)         \
    {                                                                                              \
        return (void*)(&((const effect_type*)instance)->member_name);                              \
    }                                                                                              \
    static Ursine::Json serialize_##effect_type##member_name(void *instance)                       \
    {                                                                                              \
        return Ursine::JsonSerializer::Serialize(                                                  \
            *static_cast<decltype(effect_type::member_name)*>(instance));                          \
    }                                                                                              \
    static void deserialize_##effect_type##member_name(const Ursine::Json &data, void *instance)   \
    {                                                                                              \
        Ursine::JsonSerializer::Deserialize(data,                                                  \
            *static_cast<decltype(effect_type::member_name)*>(instance));                          \
    }                                                                                              \
    volatile static Ursine::ParticleEffectType<effect_type>                                        \
        register_member_##effect_type##member_name(                                                \
            #member_name,                                                                          \
            access_##effect_type##member_name,                                                     \
            serialize_##effect_type##member_name,                                                  \
            deserialize_##effect_type##member_name,                                                \
            ##__VA_ARGS__);                                                                        \

#endif

// Allows the serializer to access private effect data. Use this before
// any private fields in the class you wish to allow access.
#define ParticleEffectSerializerAllow(effect_type, member_name) friend void *access_##effect_type##member_name(const Ursine::ParticleEffect * const instance)

// Gets the unique id of this component type
#define GetParticleEffectID(effect_type) Ursine::ParticleEffectRegistrar::GetID<effect_type>()

// Gets the unique mask for this effect type
#define GetParticleEffectMask(effect_type) Ursine::ParticleEffectRegistrar::GetMask<effect_type>()

// Determines if a particle effect of this type has been registered
#define IsParticleEffectRegistered(effect_type) Ursine::ParticleEffectRegistrar::IsRegistered<effect_type>()

// Cleaner way of calling a base component constructor in the member 
// initialization list (used to assign the component type id)
#define BaseParticleEffect() Ursine::ParticleEffect(GetParticleEffectID(std::remove_reference<decltype(*this)>::type))

namespace Ursine
{
    class JsonSerializer;
    class ParticleEffect;

    typedef void *(*ParticleEffectDataAccessor)(const ParticleEffect *);
    typedef ParticleEffect *(*ParticleEffectFactory)(void);

    typedef std::unordered_map<
        std::string,
        std::pair <ParticleEffectDataAccessor, JsonSerializer>
    > ParticleEffecDataMap;

    // Type for unique ids mapped to particle effect types
    typedef uint8 ParticleEffectTypeID;

    // Type for IDs representing unique instances of particle effects
    typedef uint32 ParticleEffectUniqueID;

    // Type for unique bit masks mapped to particle effect types
    typedef uint64 ParticleEffectTypeMask;

    // Vector of particle effect pointers
    typedef std::vector<ParticleEffect*> ParticleEffectVector;

    // Maximum number of particle able to be stored (number of bits able
    // to be stored in ParticleEffectTypeMask).
    const uint8 kMaxParticleEffectCount = sizeof(ParticleEffectTypeMask) * kBitsPerByte;

    const std::string kParticleEmitterKeyType = ".type";
}