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

#include "UIScreensConfigComponent.h"

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
    ENTITY_SYSTEM;

public:
    PlayerLookAtSystem(ursine::ecs::World* world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(const ursine::ecs::EntityHandle& entity) override;
    void Disable(const ursine::ecs::EntityHandle& entity) override;

private:
    UIScreensConfig *m_ui;

    std::unordered_map< ursine::ecs::EntityHandle, int > m_playerIDs;
    std::unordered_map< ursine::ecs::EntityHandle, RaycastComponent* > m_raycastComps;
    std::unordered_map< ursine::ecs::EntityHandle, PlayerLookAt* > m_playerLookAtComps;

    void OnSceneReady(ursine::Scene *scene) override;

} Meta(Enable, AutoAddEntitySystem);



