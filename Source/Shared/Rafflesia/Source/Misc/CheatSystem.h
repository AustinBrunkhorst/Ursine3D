/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CheatSystem.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>

#include <KeyboardManager.h>

class CheatSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    CheatSystem(ursine::ecs::World *world);

private:
    void OnInitialize(void) override;
    void OnRemove(void) override;

    void onUpdate(EVENT_HANDLER(ursine::ecs:::World));

    void skipSegmentCheat(ursine::KeyboardManager *kbManager);

} Meta(Enable);
