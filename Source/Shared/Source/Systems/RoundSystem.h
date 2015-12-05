#pragma once

#include <EntitySystem.h>


        class RoundSystem : public ursine::ecs::EntitySystem
        {
            ENTITY_SYSTEM;

        public:
            RoundSystem(ursine::ecs::World *world);

        private:
            //void OnInitialize(void) override;
            //void OnRemove(void) override;
            //
            //void onUpdate(EVENT_HANDLER(ursine::ecs:::World));

        } Meta(Enable);
