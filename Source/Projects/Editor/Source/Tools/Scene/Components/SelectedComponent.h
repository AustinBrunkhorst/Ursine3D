/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SelectedComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
    void tryDebugModel(bool enabled);

} Meta(Enable, WhiteListMethods, /*HiddenInInspector,*/ DisableSerialization);
