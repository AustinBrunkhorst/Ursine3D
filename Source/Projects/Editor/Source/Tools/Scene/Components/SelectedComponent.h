#pragma once

#include <Component.h>

class Selected : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:
    Meta(Enable)
    Selected(void);
    ~Selected(void);

    void OnInitialize(void) override;

private:
    float m_emissive;

    void tryDebugModel(bool enabled);

} Meta(Enable, WhiteListMethods, HiddenInInspector, DisableSerialization);
