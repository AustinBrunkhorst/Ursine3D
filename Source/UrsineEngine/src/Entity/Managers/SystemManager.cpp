/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SystemManager.h"

namespace Ursine
{
    namespace ECS
    {
        SystemManager::SystemManager(World *world)
            : WorldManager(world)
            , EventDispatcher(this)
        {
            auto factories = SystemRegistrar::GetFactories();

            _systems.resize(factories.size(), nullptr);

            for (auto factory : factories)
            {
                // doesn't want to be auto added
                if (!factory.second.first)
                    continue;

                auto system = factory.second.second(_world);

                system->_type_id = factory.first;
                system->_type_mask = 1ull << system->_type_id;

                _systems[static_cast<uint>(system->_type_id)] = system;
            }
        }

        SystemManager::~SystemManager(void)
        {
            auto systems_sorted(_systems);

            // sort based on priority
            std::sort(systems_sorted.begin(), systems_sorted.end(), &EntitySystem::compare);

            // cleanup systems created from their factory constructor (in reverse order)
            for (auto it = systems_sorted.rbegin(); it != systems_sorted.rend(); ++it)
            {
                auto *system = *it;

                if (system)
                {
                    system->OnRemove();

                    delete system;
                }
            }
        }

        EntitySystem *SystemManager::AddSystem(const std::string &name)
        {
            auto id = SystemRegistrar::GetID(name);

            auto &system = _systems[static_cast<uint>(id)];

            UAssert(system == nullptr, "System already added to world.");

            system = SystemRegistrar::GetFactories()[id].second(_world);

            system->_type_id = id;
            system->_type_mask = 1ull << id;

            system->OnInitialize();

            return system;
        }

        void SystemManager::OnInitialize(void)
        {
            auto systems_sorted(_systems);

            // sort based on priority
            std::sort(systems_sorted.begin(), systems_sorted.end(), &EntitySystem::compare);

            for (auto system : systems_sorted)
            {
                if (system)
                    system->OnInitialize();
            }
        }

        void SystemManager::OnWorldLoaded(void)
        {
            // notify systems that are interested
            Dispatch(SM_WORLD_LOADED, EventArgs::Empty);
        }

        void SystemManager::Update(void)
        {
            // notify systems that are interested
            Dispatch(SM_UPDATE, EventArgs::Empty);
        }

        void SystemManager::Draw(void)
        {
            // notify systems that are interested
            Dispatch(SM_DRAW, EventArgs::Empty);
        }
    }
}