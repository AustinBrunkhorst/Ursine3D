#pragma once

#include <Component.h>

class Spawnpoint : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    Spawnpoint(void);

    int teamNumber;

    int roundSpawnNnumber;
    
} Meta(Enable, DisplayName("Spawnpoint"));
