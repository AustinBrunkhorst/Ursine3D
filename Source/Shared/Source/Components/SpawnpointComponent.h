#pragma once

#include <Component.h>

class Spawnpoint : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    Spawnpoint(void);

    int teamNumber;

    Meta(Disable)
    bool alreadyUsed;
    
} Meta(Enable, DisplayName("Spawnpoint"));
