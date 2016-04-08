#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** HealthRegenSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <FilterSystem.h>

//////////////////////
////  Predefines  ////
//////////////////////

class HealthRegen;
class Health;

//////////////////////
//////////////////////


class HealthRegenSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM ;

public:
    HealthRegenSystem(ursine::ecs::World* world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(const ursine::ecs::EntityHandle& entity) override;
    void Disable(const ursine::ecs::EntityHandle& entity) override;

private:

    void CalculateHealthToAdd(Health* healthComp, HealthRegen* regenComp);

    std::unordered_map< ursine::ecs::EntityHandle, HealthRegen* > m_healthRegens;
    std::unordered_map< ursine::ecs::EntityHandle, Health* > m_healthComps;
} Meta(Enable, AutoAddEntitySystem);



