/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentSerializer.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ComponentSerializer.h"
#include "Component.h"
#include "EntityConfig.h"

namespace ursine
{
    namespace ecs
    {
        void ComponentSerializer::Serialize(Json &target, const Component *component)
        {
            Json obj = Json::object {};
            Json::object &obj_items = const_cast<Json::object&>(obj.object_items());

            auto reflection = ComponentRegistrar::Reflect(component->GetTypeID());

            for (auto member : reflection.members)
            {
                auto member_ptr = member.second.first(component);

                obj_items[member.first] = member.second.second.SerializeFn(member_ptr);
            }

            target = obj;
        }

        void ComponentSerializer::Deserialize(const Json &obj, Component *target)
        {
            UAssert(obj.is_object(), "Root must be an object.");

            auto reflection = ComponentRegistrar::Reflect(target->GetTypeID());

            int handled = 0;

            for (auto item : obj.object_items())
            {
                // reserved keys
                if (item.first == kEntityKeyBase || item.first == kEntityKeyType)
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
        }
    }
}
