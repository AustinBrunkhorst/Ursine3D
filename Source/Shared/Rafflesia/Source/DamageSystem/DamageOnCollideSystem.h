#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
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
    void Enable(const ursine::ecs::EntityHandle &entity) override;
    void Disable(const ursine::ecs::EntityHandle &entity) override;

private:

    std::unordered_map<ursine::ecs::EntityHandle, DamageOnCollide*> m_damage;
} Meta(Enable);



