/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEffectSerializer.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ParticleEffectSerializer.h"
#include "ParticleEffect.h"
#include "ParticleEffectRegistrar.h"
#include "ParticleEffectConfig.h"

namespace Ursine
{
    Json ParticleEffectSerializer::Serialize(const ParticleEffect *effect)
    {
        Json obj = Json::object{};
        Json::object &obj_items = const_cast<Json::object&>(obj.object_items());

        auto reflection = ParticleEffectRegistrar::Reflect(effect->GetTypeID());

        for (auto member : reflection.members)
        {
            auto member_ptr = member.second.first(effect);

            obj_items[member.first] = member.second.second.SerializeFn(member_ptr);
        }

        return obj;
    }

    ParticleEffect *ParticleEffectSerializer::Deserialize(const Json &obj)
    {
        UAssert(obj.is_object(), "Root must be an object.");

        auto name = obj[kParticleEmitterKeyType].string_value();

        auto target = ParticleEffectRegistrar::Instantiate(name);

        auto reflection = ParticleEffectRegistrar::Reflect(target->GetTypeID());

        int handled = 0;

        for (auto item : obj.object_items())
        {
            // reserved keys
            if (item.first == kParticleEmitterKeyType)
                continue;

            auto member = reflection.members.find(item.first);

            // we aren't interested in this field
            if (member == reflection.members.end())
            {
#ifdef CONFIG_DEBUG
                UError("Unrecognized component member found: %s\nWhile deserializing %s",
                    item.first.c_str(), reflection.name.c_str());
#else
                continue;
#endif
            }

            auto member_ptr = member->second.first(target);

            member->second.second.DeserializeFn(item.second, member_ptr);

            ++handled;
        }

#ifdef not//CONFIG_DEBUG
        UAssert(handled == reflection.members.size(),
            "Missing component member(s).\nWhile deserializing %s",
            reflection.name.c_str());
#endif

        return target;
    }
}
