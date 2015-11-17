#pragma once

#include <Component.h>

class EditorIcon : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:
    Meta(Enable)
    EditorIcon(void);
    ~EditorIcon(void);

    void SetIcon(const std::string &text);

    void OnInitialize(void) override;

private:
    ursine::ecs::Entity *m_billboard;

} Meta(
    Enable, 
    WhiteListMethods, 
    DisableSerialization, 
    HiddenInInspector
);