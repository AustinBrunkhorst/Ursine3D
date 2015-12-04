#pragma once

#include <Component.h>
#include <Vec2.h>

class PlayerInput : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    PlayerInput(void);

    bool Jump(void);

    int KillTeamCheat(void);
    
    ursine::Vec2 MoveDir(void);

    ursine::Vec2 LookDir(void);

    bool Fire( void );

    bool ResetTrigger( void );

    int id;
    bool keyboard;

    const float triggerDepressValue;
    const float triggerPressValue;
} Meta(Enable, DisplayName("PlayerInput"));