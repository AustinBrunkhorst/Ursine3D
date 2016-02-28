#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <FilterSystem.h>

//////////////////////
////  Predefines  ////
//////////////////////

class DamageOnCollide;

//////////////////////
//////////////////////


class DamageOnCollideSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM ;

public:
    DamageOnCollideSystem(ursine::ecs::World* world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(ursine::ecs::Entity* entity) override;
    void Disable(ursine::ecs::Entity* entity) override;

private:

    // entity's components changed
    void OnComponentAdded(EVENT_HANDLER(World));

    // entity removed
    void OnEntityRemoved(EVENT_HANDLER(World));

    std::unordered_map< ursine::ecs::EntityUniqueID, DamageOnCollide* > m_damage;
} Meta(Enable);



