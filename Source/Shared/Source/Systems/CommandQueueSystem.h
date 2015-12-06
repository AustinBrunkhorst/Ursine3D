/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandQueueSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>
#include <Components/CommandQueueComponent.h>

class CommandQueueSystem
    : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    CommandQueueSystem(ursine::ecs::World *world);

private:
    void OnInitialize(void) override;
    void OnRemove(void) override;

    // used to maintain player count and spawnpoint list
    void onComponentAdded(EVENT_HANDLER(ursine::ecs:::World));

    // spawn points and player count
    void onComponentRemoved(EVENT_HANDLER(ursine::ecs::World));

    void onWorldUpdate(EVENT_HANDLER(ursine::ecs::World));


    void Process(CommandQueue* commandQueue);

    std::list<CommandQueue *> m_commandQueueList;

} Meta(Enable);