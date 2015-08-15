/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntitySerializer.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "EntitySerializer.h"
#include "EntityManager.h"
#include "Entity.h"

#include "ComponentSerializer.h"

#include "ContentManager.h"

#include "Json.h"

namespace Ursine
{
    namespace ECS
    {
        EntitySerializer::~EntitySerializer(void)
        {
            
        }

        Json EntitySerializer::Serialize(Entity *entity) const
        {
            Json out = Json::object { { kEntityKeyComponents, Json::array { } } };

            Json::object &out_items = const_cast<Json::object&>(out.object_items());

            auto &tag = entity->GetTag();

            // set the tag
            if (!tag.empty())
                const_cast<Json::object&>(out.object_items())[kEntityKeyTag] = tag;

            auto &components = const_cast<Json::array &>(out[kEntityKeyComponents].array_items());
           
            for (auto *component : entity->GetComponents())
            {
                Json obj;

                ComponentSerializer::Serialize(obj, component);

                Json::object &obj_items = const_cast<Json::object&>(obj.object_items());

                obj_items[kEntityKeyType] = component->GetName();

                components.push_back(obj);
            }

            return out;
        }

        void EntitySerializer::Deserialize(const std::string &name, 
            EntityManager *manager, Entity *entity)
        {
            auto base = getIdentifier(name, manager);

            auto parent(name);

            while (!parent.empty())
            {
                auto identifier = getIdentifier(parent, manager);

                auto cache = _cache.find(identifier);

                if (cache == _cache.end())
                {
                    auto data = loadIdentifier(identifier);

                    ComponentVector created;

                    deserialize(data, manager, entity, created);

                    auto &cache_base = _cache[base];

                    for (auto component : created)
                    {
                        cache_base.push_back(component);

                        manager->addComponent(entity, 
                            ComponentRegistrar::Copy(component));
                    }

                    if (identifier != base)
                    {
                        auto &cache_identifier = _cache[identifier];

                        for (auto component : created)
                            cache_identifier.push_back(component);
                    }

                    parent = data[kEntityKeyBase].string_value();
                }
                else
                {
                    auto &components = cache->second;

                    // if this isn't the base level, copy the cache to it
                    if (identifier != base)
                    {
                        auto &cache_base = _cache[base];

                        for (auto component : components)
                            cache_base.push_back(component);
                    }

                    copyComponents(entity, manager, components);

                    // once we've hit cache, there is no point in continuing
                    break;
                }
            }
        }

        void EntitySerializer::Deserialize(const std::string &name,
            const Json &merge, EntityManager *manager, Entity *entity)
        {
            auto &tag = merge[kEntityKeyTag];

            // attempt to set the entity's tag
            if (tag.is_string())
                entity->SetTag(tag.string_value());

            // deserialize the base type first
            Deserialize(name, manager, entity);

            ComponentVector created;

            // deserialize the merged components
            deserialize(merge, manager, entity, created, true);

            for (auto component : created)
            {
                // remove the component created from the base type
                manager->removeComponent(entity, component->GetTypeID());

                // add the component created from the merged json
                manager->addComponent(entity, component);
            }
        }

        void EntitySerializer::ClearCache(void)
        {
            /*for (auto &cache : _cache)
            {
                for (auto component : cache.second)
                    delete component;
            }

            _cache.clear( );*/
        }

        void EntitySerializer::deserialize(const Json &data,
            EntityManager *manager, 
            Entity *entity, 
            ComponentVector &created_out, 
            bool no_check)
        {
            auto &components = data[kEntityKeyComponents];

            for (auto component : components.array_items())
            {
                auto &type = component[kEntityKeyType];

                UAssert(type.is_string(),
                    "Invalid or missing component type.");

                auto &type_str = type.string_value();

                ComponentTypeMask mask;

                UAssert(ComponentRegistrar::GetMask(type_str, mask),
                    "Unknown component type: %s", type_str.c_str());

                // add this component if it doesn't exist (or forced with no_check)
                if (no_check || !entity->HasComponent(mask))
                {
                    auto instance = ComponentRegistrar::Instantiate(type_str);

                    ComponentSerializer::Deserialize(component, instance);

                    created_out.push_back(instance);
                }
            }
        }

        void EntitySerializer::copyComponents(Entity *entity,
            EntityManager *manager, const ComponentVector &components)
        {
            auto size = components.size();

            for (uint32 i = 0; i < size; ++i)
            {
                if (!entity->HasComponent(components[i]->GetTypeMask()))
                {
                    manager->addComponent(entity,
                        ComponentRegistrar::Copy(components[i]));
                }
            }
        }

        std::string EntitySerializer::getIdentifier(const std::string &name, EntityManager *manager)
        {
            return manager->GetDataPath() + name + kEntityExtension;
        }

        Json EntitySerializer::loadIdentifier(const std::string &identifier)
        {
            auto text = gContentManager->LoadText(identifier);

            std::string error;

            auto json = Json::parse(*text, error);

            UAssert(error.empty(), 
                "Error parsing Entity JSON (%s)\n%s", 
                identifier.c_str(), error.c_str());

            return json;
        }
    }
}
