/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectType.hpp
** - Stores particle effect type information
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "ParticleEffectRegistrar.h"

namespace ursine
{
    template<class Type>
    ParticleEffectTypeID ParticleEffectType<Type>::ID = 0;

    template<class Type>
    ParticleEffectTypeMask ParticleEffectType<Type>::Mask = 0;

    template<class Type>
    bool ParticleEffectType<Type>::Registered = false;

    template<class Type>
    ParticleEffectType<Type>::ParticleEffectType(const char *name)
    {
        ParticleEffectRegistrar::Register<Type>(name);
    }

    template<class Type>
    ParticleEffectType<Type>::ParticleEffectType(
        const char *member_name,
        ParticleEffectDataAccessor accessor,
        JsonSerializer::Serializer serializer,
        JsonSerializer::Deserializer deserializer,
        const char *display_name = nullptr)
    {
        ParticleEffectRegistrar::RegisterMember<Type>(display_name ? display_name : member_name, accessor,
            serializer, deserializer);
    }

    template<class Type>
    ParticleEffect *ParticleEffectType<Type>::Create(void)
    {
        // by default, use the default constructor
        return new Type();
    }

    template<class Type>
    ParticleEffect *ParticleEffectType<Type>::Copy(const ParticleEffect *instance)
    {
        static_assert(std::is_copy_constructible<Type>::value,
            "This component type doesn't have a copy constructor!");

        // by default, use the copy constructor
        return new Type(*(static_cast<const Type*>(instance)));
    }
}
