/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorFocusPoint.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class EntityAnimatorFocusPoint : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EntityAnimatorFocusPoint(void);

} Meta(Enable);
