#pragma once

#include <Component.h>
#include <Core/Graphics/API/GfxAPI.h>

class Selected : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:
    Selected(void);
    ~Selected(void);

    void OnInitialize(void) override;

private:
    float m_emissive;

    void tryDebugModel(bool enabled);

} Meta(Enable, HiddenInInspector);