/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraAnimatorFocusPointComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class CameraAnimatorFocusPoint : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    CameraAnimatorFocusPoint(void);

} Meta(Enable);
