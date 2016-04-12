#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerLookAtSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <FilterSystem.h>

//////////////////////
////  Predefines  ////
//////////////////////

class PlayerLookAt;
class RaycastComponent;

//////////////////////
//////////////////////


class PlayerLookAtSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM ;

public:
    PlayerLookAtSystem(ursine::ecs::World* world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(const ursine::ecs::EntityHandle& entity) override;
    void Disable(const ursine::ecs::EntityHandle& entity) override;

private:
    std::unordered_map< ursine::ecs::EntityHandle, int > m_playerIDs;
    std::unordered_map< ursine::ecs::EntityHandle, RaycastComponent* > m_raycastComps;
    std::unordered_map< ursine::ecs::EntityHandle, PlayerLookAt* > m_playerLookAtComps;

} Meta(Enable, AutoAddEntitySystem);



