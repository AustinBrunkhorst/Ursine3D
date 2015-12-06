/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CharacterFireControllerSystem.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#pragma once

#include <EntitySystem.h>
#include <list>
#include <Randomizer.h>
#include <Components/CharacterFireControllerComponent.h>

class CharacterFireControllerSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    CharacterFireControllerSystem( ursine::ecs::World *world );


private:

    void OnInitialize(void) override;
    void OnRemove(void) override;

    //void onRoundOver(EVENT_HANDLER(ursine::ecs:::World));

    // used to maintain player count and spawnpoint list
    void onComponentAdded(EVENT_HANDLER(ursine::ecs:::World));

    // spawn points and player count
    void onComponentRemoved(EVENT_HANDLER(ursine::ecs::World));

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    void Process(CharacterFireController *fireController);

    std::list<CharacterFireController *> m_fireControllers;

    ursine::Randomizer m_rng;

} Meta( Enable );