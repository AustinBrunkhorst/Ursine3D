/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandInputControllerSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>

class CommandInputControllerSystem 
    : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    CommandInputControllerSystem(ursine::ecs::World *world);

protected:

    void OnInitialize(void) override;
    void OnRemove(void) override;

private:

    void onComponentAdded(EVENT_HANDLER(ursine::ecs::World));
    void onComponentRemoved(EVENT_HANDLER(ursine::ecs::World));

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    void Process(ursine::ecs::Entity *entity);

    std::vector<ursine::ecs::Entity *> m_entities;

} Meta(Enable);