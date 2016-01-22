/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorIconComponent.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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