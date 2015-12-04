#pragma once

#include <Component.h>

class TeamComponent : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    TeamComponent(void);

    int TeamNumber;
    
} Meta(Enable, DisplayName("Team"));
