#pragma once

#include <EntitySystem.h>


        class CheatSystem : public ursine::ecs::EntitySystem
        {
            ENTITY_SYSTEM;

        public:
            CheatSystem(ursine::ecs::World *world);

        private:
            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onUpdate(EVENT_HANDLER(ursine::ecs:::World));

            int killTeamCheat(void);

        } Meta(Enable);
