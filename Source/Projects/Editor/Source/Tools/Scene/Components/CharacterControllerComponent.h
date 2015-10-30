#pragma once

#include <Component.h>
#include <UrsineLogTools.h>

URSINE_TODO("REMOVE THIS FUCKER AFTER PRESENTATION.");
class CharacterController : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    CharacterController(void);
            
    float speed;
    int id;
} Meta(Enable, DisplayName("CharacterController"));
