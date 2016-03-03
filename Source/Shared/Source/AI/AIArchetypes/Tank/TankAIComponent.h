/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TankAIComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "TankAIStateMachine.h"

class TankAI : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
  TankAI(void);
  ~TankAI(void);

private:

    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(World));


    ursine::sm::TankAIStateMachine m_stateMachine;

} Meta(Enable);
