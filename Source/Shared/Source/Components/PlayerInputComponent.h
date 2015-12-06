/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerInputComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <Vec2.h>

class PlayerInput : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    PlayerInput(void);

    bool Jump(void);
    
    ursine::Vec2 MoveDir(void);

    ursine::Vec2 LookDir(void);

    bool Fire( void );

    bool ResetTrigger( void );

    int id;
    bool keyboard;

    const float triggerDepressValue;
    const float triggerPressValue;
} Meta(Enable, DisplayName("PlayerInput"));