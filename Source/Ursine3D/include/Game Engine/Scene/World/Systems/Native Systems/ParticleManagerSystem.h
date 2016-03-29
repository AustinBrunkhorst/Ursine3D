/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleManagerSystem.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "EntitySystem.h"

namespace ursine
{
    namespace ecs
    {
        class ParticleSystem;

        class ParticleManagerSystem : public EntitySystem
        {
            ENTITY_SYSTEM;

        public:
            Meta(Enable)
            ParticleManagerSystem(World *world);

        private:
            std::vector<ParticleSystem *> m_systems;

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

        #if defined(URSINE_WITH_EDITOR)

            void onEditorUpdate(EVENT_HANDLER(World));

        #endif
        } Meta(
            Enable,
            WhiteListMethods,
            AutoAddEntitySystem
        );
    }
}
