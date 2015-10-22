#pragma once

#include <Component.h>

class Selected : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:
    Selected(void);
} Meta(Enable, HiddenInInspector);