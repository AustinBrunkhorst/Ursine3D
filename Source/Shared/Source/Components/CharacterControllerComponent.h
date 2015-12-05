#pragma once

#include <Component.h>
#include <UrsineLogTools.h>

class CharacterController : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    CharacterController(void);
    
    float moveSpeed;
    float jumpSpeed;
	float rotateSpeed;

    // TEMPORARY I AM SO SORRY
    ursine::SVec3 position;
    ursine::Vec2 moveDir;
    ursine::Vec2 lookDir;
    bool jump;
} Meta(Enable, DisplayName("CharacterController"));
