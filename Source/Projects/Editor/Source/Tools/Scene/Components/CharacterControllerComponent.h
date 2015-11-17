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
    int id;

} Meta(Enable, DisplayName("CharacterController"));
