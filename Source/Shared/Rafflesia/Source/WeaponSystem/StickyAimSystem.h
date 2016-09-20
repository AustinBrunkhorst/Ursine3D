#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** StickyAimSytem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <FilterSystem.h>

//////////////////////
////  Predefines  ////
//////////////////////

class RaycastComponent;
class CharacterController;

namespace ursine
{
    namespace ecs
    {
        class PhysicsSystem;
    }
}

//////////////////////
//////////////////////


class StickyAimSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM ;

public:
    StickyAimSystem(ursine::ecs::World* world);

protected:
    void Initialize(void) override;
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(const ursine::ecs::EntityHandle& entity) override;
    void Disable(const ursine::ecs::EntityHandle& entity) override;

private:
    std::unordered_map< ursine::ecs::EntityHandle, RaycastComponent* > m_raycastComps;
    std::unordered_map< ursine::ecs::EntityHandle, CharacterController* > m_charControllers;

    ursine::ecs::PhysicsSystem *m_physicsSystem;

} Meta(Enable, AutoAddEntitySystem);



