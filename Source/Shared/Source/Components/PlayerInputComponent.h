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

    int id;
    bool keyboard;
} Meta(Enable, DisplayName("PlayerInput"));