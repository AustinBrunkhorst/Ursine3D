#pragma once

#include "Component.h"
#include <Core/Graphics/API/GfxAPI.h>

class Selector : public ursine::ecs::Component
{
    NATIVE_COMPONENT

public:
    Selector(void);
    ~Selector(void);

    void OnInitialize(void) override;

private:
    ursine::graphics::GfxHND m_boxHandle;
}Meta(Enable, DisplayName("Selector"));